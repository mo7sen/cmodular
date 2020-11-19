#include "hashmap.h"
#include "module.h"
#include <modulesystem.h>
#include <stdio.h>
#include <assert.h>

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

  return 0;

failedcats:
  hashmap_free(system_modules);
failedmods:
  fprintf(stderr, "Failed to allocate memory for modulesystem\n");
  return 1;
}

void modulesystem_deinit(modulesystem_t *modulesystem)
{
  hashmap_free(modulesystem->modules);
  hashmap_free(modulesystem->categories);

  modulesystem->modules = 0;
  modulesystem->categories = 0;
}

int32_t modulesystem_addmodule(modulesystem_t *modulesystem, module_t *module)
{
  string_t depname;
  int32_t idx;
  vec_foreach(&module->metadata.category_dependencies, depname, idx)
  {
    if(!modulesystem_hascategory(modulesystem, depname))
    {
      assert(!"CategoryDependency not met");
    }
  }
  vec_foreach(&module->metadata.module_dependencies, depname, idx)
  {
    if(!modulesystem_hasmodule(modulesystem, depname))
    {
      assert(!"ModuleDependency not met");
    }
  }

  if(modulesystem_getmodule(modulesystem, module->metadata.name))
  {
    assert(!"Found module with the same name");
  }

  void *replaced_element = hashmap_set(modulesystem->modules, module);
  if(!replaced_element && hashmap_oom(modulesystem->modules))
    goto failed_hashmap_set; // Out of memory

  return 0;

failed_hashmap_set:
  fprintf(stderr, "Couldn't add module \"%s\" to modulesystem. Out of Memory\n", module->metadata.name);
  return 1;

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
