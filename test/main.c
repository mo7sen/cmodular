#include <stdio.h>
#include <stdint.h>

#include <cmodular.h>
#include "cmod_categories.h"
#include "module.h"

uint32_t mathmodule_init() 
{
  printf("Mathmodule Init\n");
  return 0;
}

uint32_t mathmodule_deinit() 
{
  printf("Mathmodule deinit\n");
  return 0;
}

uint32_t mathmodule_run() 
{
  for(int i = 0; i < 1000; ++i)
    printf("Mathmodule Run. i = %d\n", i);
  return 0;
}

uint32_t renderingmodule_init()
{
  printf("renderingmodule, Init\n");
  return 0;
}

uint32_t renderingmodule_deinit()
{
  printf("renderingmodule deinit\n");
  return 0;
}

uint32_t renderingmodule_run()
{
  for(int i = 0; i < 1000; ++i)
  {
    printf("renderingmodule Run. i = %d\n", i);
  }
  return 0;
}

uint32_t physicsmodule_init()
{
  printf("Physicsmodule, Init\n");
  return 0;
}

uint32_t physicsmodule_deinit()
{
  printf("Physicsmodule deinit\n");
  return 0;
}

uint32_t physicsmodule_run()
{
  for(int i = 0; i < 1000; ++i)
  {
    printf("Physicsmodule Run. i = %d\n", i);
  }
  return 0;
}

int main()
{
  // Initialize the modulesystem
  modulesystem_t modulesystem;
  modulesystem_init(&modulesystem);

  // Create a new module
  module_t MathModule;
  module_t PhysicsModule;
  module_t RenderingModule;
  module_create(&MathModule, "MathModule");
  module_create(&PhysicsModule, "PhysicsModule");
  module_create(&RenderingModule, "RenderingModule");

  module_bindfunction(&MathModule, BaseCategory, init, mathmodule_init);
  module_bindfunction(&MathModule, BaseCategory, run, mathmodule_run);
  module_bindfunction(&MathModule, BaseCategory, deinit, mathmodule_deinit);

  module_bindfunction(&RenderingModule, BaseCategory, init,   renderingmodule_init);
  module_bindfunction(&RenderingModule, BaseCategory, run,    renderingmodule_run);
  module_bindfunction(&RenderingModule, BaseCategory, deinit, renderingmodule_deinit);

  module_bindfunction(&PhysicsModule, BaseCategory, init,   physicsmodule_init);
  module_bindfunction(&PhysicsModule, BaseCategory, run,    physicsmodule_run);
  module_bindfunction(&PhysicsModule, BaseCategory, deinit, physicsmodule_deinit);

  modulesystem_addmodule(&modulesystem, &MathModule);
  modulesystem_addmodule(&modulesystem, &PhysicsModule);
  modulesystem_addmodule(&modulesystem, &RenderingModule);

  modulesystem_start(&modulesystem);

  // Deinitialize the modulesystem
  // N.B. modules are automatically destroyed when the modulesystem_deinit is
  // called
  modulesystem_deinit(&modulesystem);
}
