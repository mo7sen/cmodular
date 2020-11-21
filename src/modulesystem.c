#include "hashmap.h"
#include "module.h"
#include <modulesystem.h>
#include <category.h>
#include <common.h>
#include <evolthreadpool.h>
#include <evolpthreads.h>
#include <pthread.h>

int string_cmp(const void *cat1, const void *cat2, void *udata) { (void)udata; return strcmp(cat1, cat2); }
uint64_t string_hash(const void *cat, uint64_t seed0, uint64_t seed1) { return hashmap_murmur(cat, strlen(cat), seed0, seed1); }

int32_t modulesystem_init(modulesystem_t *modulesystem)
{
  struct hashmap *system_modules;
  struct hashmap *system_categories;

  system_modules = hashmap_new(sizeof(module_t), 0, 0, 0, module_hash, module_cmp, NULL);
  if(!system_modules)
    goto failedmods;

  system_categories = hashmap_new(sizeof(string_t), 0, 0, 0,  string_hash, string_cmp, NULL);
  if(!system_categories)
    goto failedcats;

  modulesystem->modules = system_modules;
  modulesystem->categories = system_categories;

  return CMOD_SUCCESS;

failedcats:
  hashmap_free(system_modules);
failedmods:
  cmod_err("Failed to allocate memory for modulesystem\n");
  return CMOD_OOM;
}

bool module_destroy_iter(const void *module, void *udata)
{
  (void)udata;
  module_destroy((module_t*)module);
  return true;
}

void modulesystem_deinit(modulesystem_t *modulesystem)
{
  hashmap_scan(modulesystem->modules, module_destroy_iter, NULL);
  hashmap_free(modulesystem->modules);
  hashmap_free(modulesystem->categories);

  modulesystem->modules = 0;
  modulesystem->categories = 0;
}

int32_t modulesystem_addmodule(modulesystem_t *modulesystem, module_t *module)
{
  if(modulesystem_getmodule(modulesystem, module->metadata.name))
  {
    cmod_err("Module name collision \"%s\"", module->metadata.name);
    goto module_name_collision;
  }

  string_t depname;
  int32_t idx;
  vec_foreach(&module->metadata.category_dependencies, depname, idx)
  {
    if(!modulesystem_hascategory(modulesystem, depname))
    {
      cmod_err("Category dependency \"%s\" not found while adding module \"%s\"", depname, module->metadata.name);
      goto dependency_not_found;
    }
  }
  vec_foreach(&module->metadata.module_dependencies, depname, idx)
  {
    if(!modulesystem_hasmodule(modulesystem, depname))
    {
      cmod_err("Module dependency \"%s\" not found while adding module \"%s\"", depname, module->metadata.name);
      goto dependency_not_found;
    }
  }

  void *replaced_element = hashmap_set(modulesystem->modules, module);
  if(!replaced_element && hashmap_oom(modulesystem->modules))
    goto failed_hashmap_set; // Out of memory

  return CMOD_SUCCESS;

module_name_collision:
  return CMOD_ERR_ADDMODULE_NAMECOLLISION;

dependency_not_found:
  return CMOD_ERR_ADDMODULE_DEPENDENCYNOTFOUND;

failed_hashmap_set:
  cmod_err("Couldn't add module \"%s\" to modulesystem. Out of Memory\n", module->metadata.name);
  return CMOD_OOM;
}

module_t *modulesystem_getmodule(modulesystem_t *modulesystem, const string_t modulename)
{
  return hashmap_get(modulesystem->modules, &(module_t) {.metadata.name = modulename});
}

typedef struct
{
  const string_t categoryname;
  module_t *returnmodule;
} query_t;

bool get_module_with_category(const void *module, void *p_query)
{
  query_t *query = p_query;
  if(module_hascategory((module_t*)module, query->categoryname))
  {
    ((query_t*)p_query)->returnmodule = (module_t*)module;
    return false;
  }
  return true;
}

bool modulesystem_hascategory(modulesystem_t *modulesystem, const string_t categoryname)
{
  return hashmap_get(modulesystem->categories, categoryname);
}

module_t *modulesystem_getcategory(modulesystem_t *modulesystem, const string_t categoryname)
{
  query_t query = {.categoryname = categoryname};
  bool found = !hashmap_scan(modulesystem->modules, get_module_with_category, &query);
  if(found)
  {
    return query.returnmodule;
  }

  return NULL;
}


typedef struct
{
  modulesystem_t *modulesystem;
  ev_tpool_t *threadpool;
  pthread_cond_t stateUpdateCond;
  pthread_mutex_t stateMutex;
  struct hashmap *initialized_modules;
  struct hashmap *initialized_categories;
} SystemInitData;

typedef struct
{
  SystemInitData *systemData;
  module_t *module;
} ModuleInitData;

bool module_init_category_iter(const void *item, void *data)
{
  ModuleInitData *initData = (ModuleInitData *)data;
  modulecategory_t *category = (modulecategory_t *)item;

  hashmap_set(initData->systemData->initialized_categories, category->name);
  hashmap_set(initData->systemData->modulesystem->categories, category->name);

  return true;
}

void module_init_thr(void *data)
{
  ModuleInitData *initData = (ModuleInitData *)data;

  pthread_mutex_lock(&initData->systemData->stateMutex);

  // Check for dependency
  bool catDeps = false;
  bool modDeps = false;
  string_t depname = NULL;
  int32_t idx = 0;
  while(1)
  {
    if(!modDeps)
    {
      modDeps = true;
      vec_foreach(&initData->module->metadata.module_dependencies, depname, idx)
      {
        if(!hashmap_get(initData->systemData->initialized_modules, depname))
        {
          modDeps = false;
          break;
        }
      }
    }

    if(!catDeps)
    {
      catDeps = true;
      vec_foreach(&initData->module->metadata.category_dependencies, depname, idx)
      {
        if(!hashmap_get(initData->systemData->initialized_categories, depname))
        {
          catDeps = false;
          break;
        }
      }
    }

    if(!catDeps || !modDeps)
    {
      pthread_cond_wait(&initData->systemData->stateUpdateCond, &initData->systemData->stateMutex);
    }
    else
    {
      break;
    }
  }

  pthread_mutex_unlock(&initData->systemData->stateMutex);

  uint32_t (*init) () = module_getfunction(initData->module, BaseCategory, init);
  if(init) 
  {
    init();

    hashmap_set(initData->systemData->initialized_modules, initData->module->metadata.name);
    hashmap_scan(initData->module->categories, module_init_category_iter, initData);
    pthread_cond_broadcast(&initData->systemData->stateUpdateCond);
  }

  free(data);
}

bool module_init_iter(const void *mod_p, void *udata)
{
  SystemInitData *systemData = (SystemInitData *)udata;

  module_t *module = (module_t *) mod_p;

  if(module_hascategory(module, "BaseCategory"))
  {
    ModuleInitData *moduleData = malloc(sizeof(ModuleInitData));
    *moduleData = (ModuleInitData) {
      .module = module,
      .systemData = systemData,
    };
    ev_tpool_add_work(systemData->threadpool, module_init_thr, moduleData);
  }

  return true;
}

bool module_deinit_iter(const void *mod_p, void *udata)
{
  SystemInitData *systemData = (SystemInitData *)udata;

  module_t *module = (module_t *) mod_p;

  if(module_hascategory(module, "BaseCategory"))
  {
    ev_tpool_add_work(systemData->threadpool, (thread_func_t)module_getfunction(module, BaseCategory, deinit), NULL);
  }

  return true;
}

bool module_run_iter(const void *mod_p, void *udata)
{
  SystemInitData *systemData = (SystemInitData *)udata;

  module_t *module = (module_t *) mod_p;

  if(module_hascategory(module, "BaseCategory"))
  {
    ev_tpool_add_work(systemData->threadpool, (thread_func_t)module_getfunction(module, BaseCategory, run), NULL);
  }

  return true;
}

int32_t modulesystem_start(modulesystem_t *modulesystem)
{
  int32_t result;
  // Start modules in order so that dependencies are fulfilled
  SystemInitData data = (SystemInitData) {
    .modulesystem = modulesystem,
  };

  pthread_cond_init(&data.stateUpdateCond, NULL);
  pthread_mutex_init(&data.stateMutex, NULL);
  data.initialized_modules = hashmap_new(sizeof(char *), 0, 0, 0, string_hash, string_cmp, NULL);
  data.initialized_categories = hashmap_new(sizeof(char *), 0, 0, 0, string_hash, string_cmp, NULL);
  data.threadpool = ev_tpool_create(0);

  bool success = hashmap_scan(modulesystem->modules, module_init_iter, &data);
  ev_tpool_wait(data.threadpool);
  if(!success) goto init_error;

  success = hashmap_scan(modulesystem->modules, module_run_iter, &data);
  ev_tpool_wait(data.threadpool);
  if(!success) goto run_error;

  success = hashmap_scan(modulesystem->modules, module_deinit_iter, &data);
  ev_tpool_wait(data.threadpool);
  if(!success) goto deinit_error;

  goto success;

init_error:
  result = CMOD_ERR_INIT;
  goto exit;

run_error:
  result = CMOD_ERR_RUN;
  goto exit;

deinit_error:
  result = CMOD_ERR_DEINIT;
  goto exit;

success:
  result = CMOD_SUCCESS;
  goto exit;

exit:

  pthread_cond_destroy(&data.stateUpdateCond);
  pthread_mutex_destroy(&data.stateMutex);
  hashmap_free(data.initialized_modules);
  hashmap_free(data.initialized_categories);
  ev_tpool_destroy(data.threadpool);

  return result;
}
