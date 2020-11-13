#include "module.h"
#include <stdio.h>
#include <stdint.h>
#include <modulesystem.h>

int main()
{
  modulesystem_t modulesystem;
  modulesystem_init(&modulesystem);

  module_t evol_renderer = module_create("evol_renderer");
  module_addcategory(&evol_renderer, "evol");
  module_addcategory(&evol_renderer, "renderer");

  module_t evol_physics = module_create("evol_physics");
  module_addcategory(&evol_physics, "physics");

  modulesystem_addmodule(&modulesystem, evol_renderer);
  modulesystem_addmodule(&modulesystem, evol_physics);

  char *modulename;
  int idx;
  vec_foreach(&modulesystem.module_names, modulename, idx)
  {
    printf("module #%d: name = %s\n", idx, modulename);
  }

  module_t *found = modulesystem_getmodule(&modulesystem, "renderer");
  if(found)
  {
    printf("found.categories.length = %d\n", found->metadata.categories.length);
  }

  module_destroy(evol_renderer);
  module_destroy(evol_physics);
  modulesystem_deinit(&modulesystem);
  return 0;
}
