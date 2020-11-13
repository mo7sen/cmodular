#include <module.h>
#include <stdint.h>

module_t module_create(const string_t name)
{
  module_t result;
  result.metadata.name = name;
  vec_init(&result.categories);
  vec_init(&result.metadata.dependencies);

  return result;
}

void module_destroy(module_t module)
{
  vec_deinit(&module.categories);
  vec_deinit(&module.metadata.dependencies);
}

void module_addcategory(module_t *module, modulecategory_t category)
{
  vec_push(&module->categories, category);
}

bool module_hascategory(module_t *module, const string_t category_name)
{
  int idx;
  modulecategory_t category_iter;
  vec_foreach(&module->categories, category_iter, idx)
  {
    if(!strcmp(category_iter.name, category_name))
      return true;
  }
  return false;
}

int module_getcategoryidx(module_t *module, const string_t category_name)
{
  int idx;
  modulecategory_t category_iter;
  vec_foreach(&module->categories, category_iter, idx)
  {
    if(!strcmp(category_iter.name, category_name))
      return idx;
  }

  return -1;
}
