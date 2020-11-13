#include <module.h>
#include <stdint.h>

module_t module_create(const string_t name)
{
  module_t result;
  result.metadata.name = name;
  vec_init(&result.metadata.categories);
  vec_init(&result.metadata.dependencies);

  return result;
}

void module_destroy(module_t module)
{
  vec_deinit(&module.metadata.categories);
  vec_deinit(&module.metadata.dependencies);
}

void module_addcategory(module_t *module, const string_t category_name)
{
  vec_push(&module->metadata.categories, category_name);
}

bool module_hascategory(module_t *module, const string_t category_name)
{
  int idx;
  string_t category_iter;
  vec_foreach(&module->metadata.categories, category_iter, idx)
  {
    if(!strcmp(category_iter, category_name))
      return true;
  }
  return false;
}
