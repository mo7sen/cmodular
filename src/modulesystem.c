#include "hashmap.h"
#include "module.h"
#include <modulesystem.h>
#include <assert.h>

void modulesystem_init(modulesystem_t *modulesystem)
{
  modulesystem->modules    = hashmap_new(sizeof(module_t), 0, 0, 0, module_hash, module_cmp, NULL);
  modulesystem->categories = hashmap_new(sizeof(modulecategory_t), 0, 0, 0,  modulecategory_hash, modulecategory_cmp, NULL);
}

void modulesystem_deinit(modulesystem_t *modulesystem)
{
  hashmap_free(modulesystem->modules);
  hashmap_free(modulesystem->categories);
}

bool modulesystem_hasmodule(modulesystem_t *modulesystem, const string_t modulename)
{
  return hashmap_get(modulesystem->modules, &(module_t) {.metadata.name = modulename});
}

bool modulesystem_hascategory(modulesystem_t *modulesystem, const string_t categoryname)
{
  return hashmap_get(modulesystem->categories, &(modulecategory_t){.name = categoryname});
}

void modulesystem_addmodule(modulesystem_t *modulesystem, module_t *module)
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

  hashmap_set(modulesystem->modules, module);
}

module_t *modulesystem_getmodule(modulesystem_t *modulesystem, const string_t modulename)
{
  return hashmap_get(modulesystem->modules, &(module_t) {.metadata.name = modulename});
}

typedef struct
{
  modulesystem_t *modulesystem;
  const string_t categoryname;
} query_t;

bool get_category_from_modules(const void *module, void *query)
{
  modulecategory_t *category = module_getcategory((module_t*)module, ((query_t*)query)->categoryname);
  if(category)
  {
    hashmap_set(((query_t*)query)->modulesystem->categories, category);
  }
  return !category;
}

void *modulesystem_getinterface(modulesystem_t *modulesystem, const string_t categoryname)
{
  modulecategory_t *category;
  category = hashmap_get(modulesystem->categories, &(modulecategory_t){.name = categoryname});
  if(category && category->interface_instance)
  {
    return category->interface_instance;
  }

  bool miss = hashmap_scan(modulesystem->modules, get_category_from_modules,
      &(query_t){.modulesystem = modulesystem, .categoryname = categoryname});
  if(!miss)
  {
    category = hashmap_get(modulesystem->categories, &(modulecategory_t){.name = categoryname});
  }
  if(category && category->interface_instance)
  {
    return category->interface_instance;
  }

  return NULL;
}
