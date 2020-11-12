#include <modulesystem.h>

void modulesystem_init(modulesystem_t *modulesystem)
{
  vec_init(&modulesystem->modules);
}

void modulesystem_deinit(modulesystem_t *modulesystem)
{
  vec_deinit(&modulesystem->modules);
}

bool modulesystem_hasmodule(modulesystem_t *modulesystem, const string_t modulename)
{
  string_t name_iter;
  int32_t idx;
  vec_foreach(&modulesystem->module_names, name_iter, idx)
  {
    if(!strcmp(name_iter, modulename))
      return true;
  }
  return false;
}

bool modulesystem_hascategory(modulesystem_t *modulesystem, const string_t categoryname)
{
  string_t name_iter;
  int32_t idx;
  vec_foreach(&modulesystem->categories, name_iter, idx)
  {
    if(!strcmp(name_iter, categoryname))
      return true;
  }
  return false;
}

void modulesystem_addmodule(modulesystem_t *modulesystem, module_t module)
{
  string_t depname;
  int32_t idx;
  vec_foreach(&module.metadata.dependencies, depname, idx)
  {
    if(!(modulesystem_hasmodule(modulesystem, depname) || modulesystem_hascategory(modulesystem, depname)))
    {
      assert(!"Dependency not met");
    }
  }

  // check for category collision
  string_t categoryname;
  vec_foreach(&module.metadata.categories, categoryname, idx)
  {
    if(modulesystem_hascategory(modulesystem, categoryname))
    {
      assert(!"Category name collision");
    }
  }

  vec_push(&modulesystem->modules, module);

  vec_push(&modulesystem->module_names, module.metadata.name);
  vec_foreach(&module.metadata.categories, categoryname, idx)
  {
    vec_push(&modulesystem->categories, categoryname);
  }
}
