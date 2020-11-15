#include "hashmap.h"
#include "module.h"
#include <modulesystem.h>
#include <stdio.h>
#include <assert.h>

int32_t modulesystem_init(modulesystem_t *modulesystem)
{
  struct hashmap *system_modules;
  struct hashmap *system_categories;
  system_modules = hashmap_new(sizeof(module_t), 0, 0, 0, module_hash, module_cmp, NULL);

  if(!system_modules)
    goto failedmods;

  system_categories = hashmap_new(sizeof(modulecategory_t), 0, 0, 0,  modulecategory_hash, modulecategory_cmp, NULL);

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
  modulesystem_t *modulesystem;
  const string_t categoryname;
} query_t;

bool get_category_from_modules(const void *module, void *p_query)
{
  query_t *query = p_query;
  modulecategory_t *category = module_getcategory((module_t*)module, query->categoryname);
  if(category)
  {
    void *replaced_element = hashmap_set(query->modulesystem->categories, category);
    if(!replaced_element && hashmap_oom(query->modulesystem->categories))
    {
      fprintf(stderr, "Found category \"%s\" in a module but couldn't load it"
                      "into the modulesystem. Out of Memory\n", query->categoryname);
    }
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
