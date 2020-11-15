#include <module.h>
#include <stdint.h>
#include <hashmap.h>

module_t module_create(const string_t name)
{
  module_t result;
  result.metadata.name = name;
  result.categories = hashmap_new(sizeof(modulecategory_t), 0, 0, 0,  modulecategory_hash, modulecategory_cmp, NULL);
  vec_init(&result.metadata.category_dependencies);
  vec_init(&result.metadata.module_dependencies);

  return result;
}

void *module_getinterface(module_t *module, const string_t name)
{
  modulecategory_t *category = module_getcategory(module, name);
  if(category && category->interface_instance)
    return category->interface_instance;
  return NULL;
}

void module_destroy(module_t *module)
{
  hashmap_free(module->categories);
  vec_deinit(&module->metadata.category_dependencies);
  vec_deinit(&module->metadata.module_dependencies);
}

void module_addcategory(module_t *module, const string_t category_name, void *interface)
{
  modulecategory_t category = (modulecategory_t) {
    .name = category_name,
    .interface_instance = interface,
  };
  hashmap_set(module->categories, &category);
}

modulecategory_t *module_getcategory(module_t *module, const string_t categoryname)
{
  return hashmap_get(module->categories, &(modulecategory_t){.name = categoryname});
}

int modulecategory_cmp(const void *cat1, const void *cat2, void *udata)
{
  const modulecategory_t *category1 = cat1;
  const modulecategory_t *category2 = cat2;
  return strcmp(category1->name, category2->name);
}

uint64_t modulecategory_hash(const void *cat, uint64_t seed0, uint64_t seed1) 
{
  const modulecategory_t *category = cat;
  return hashmap_murmur(category->name, strlen(category->name), seed0, seed1);
}

int module_cmp(const void *mod1, const void *mod2, void *udata)
{
  const module_t *module1 = mod1;
  const module_t *module2 = mod2;
  return strcmp(module1->metadata.name, module2->metadata.name);
}

uint64_t module_hash(const void *mod, uint64_t seed0, uint64_t seed1) 
{
  const module_t *module = mod;
  return hashmap_murmur(module->metadata.name, strlen(module->metadata.name), seed0, seed1);
}

void module_adddependency(module_t *module, const string_t dependency_name, bool moduledependency)
{
  if(moduledependency)
  {
    vec_push(&module->metadata.module_dependencies, dependency_name);
  }
  else
  {
    vec_push(&module->metadata.category_dependencies, dependency_name);
  }
}
