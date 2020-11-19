#include <module.h>
#include <stdint.h>
#include <hashmap.h>
#include <interface.h>
#include <stdio.h>

int32_t module_create(module_t *module, const string_t name)
{
  struct hashmap *categories_map = 
    hashmap_new(sizeof(modulecategory_t), 0, 0, 0,  modulecategory_hash, modulecategory_cmp, NULL);
  if(!categories_map)
    goto failed_categoriesmap_creation;

  module->metadata.name = name;
  module->categories = categories_map;
  vec_init(&module->metadata.category_dependencies);
  vec_init(&module->metadata.module_dependencies);

  return 0;

failed_categoriesmap_creation:
    fprintf(stderr, "Failed to allocate memory for module \"%s\"\n", name);
    return 1;
}

void module_destroy(module_t *module)
{
  hashmap_free(module->categories);
  module->categories = 0;

  vec_deinit(&module->metadata.category_dependencies);
  vec_deinit(&module->metadata.module_dependencies);
}

int32_t module_addcategory_impl(module_t *module, const string_t category_name)
{
  modulecategory_t category = (modulecategory_t) {
    .name = category_name,
    .interface_instance = NULL,
  };
  void *replaced_element = hashmap_set(module->categories, &category);

  if(!replaced_element && hashmap_oom(module->categories))
    goto failed_category_add;

  return 0;

failed_category_add:
    fprintf(stderr, "Couldn't add category \"%s\" to module \"%s\": Out of Memory\n", 
        category_name, module->metadata.name);
    return 1;
}

modulecategory_t *module_getcategory(module_t *module, const string_t categoryname)
{
  return hashmap_get(module->categories, &(modulecategory_t){.name = categoryname});
}

int modulecategory_cmp(const void *cat1, const void *cat2, void *udata)
{
  (void)udata;
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
  (void)udata;
  const module_t *module1 = mod1;
  const module_t *module2 = mod2;
  return strcmp(module1->metadata.name, module2->metadata.name);
}

uint64_t module_hash(const void *mod, uint64_t seed0, uint64_t seed1) 
{
  const module_t *module = mod;
  return hashmap_murmur(module->metadata.name, strlen(module->metadata.name), seed0, seed1);
}

int32_t module_adddependency(module_t *module, const string_t dependency_name, bool moduledependency)
{
  int32_t result;
  if(moduledependency)
  {
    result = vec_push(&module->metadata.module_dependencies, dependency_name);
  }
  else
  {
    result = vec_push(&module->metadata.category_dependencies, dependency_name);
  }

  if(result)
  {
    fprintf(stderr, "Couldn't add %s dependency to module \"%s\": Out of Memory\n",
        moduledependency?"module":"category", module->metadata.name);
  }

  return result;
}

