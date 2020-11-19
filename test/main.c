#include <stdio.h>
#include <stdint.h>

#include <cmodular.h>
#include "cmod_categories.h"

// Implementation
uint32_t add_fn(uint32_t a, uint32_t b) { return a + b; }
uint32_t sub_fn(uint32_t a, uint32_t b) { return a - b; }
uint32_t mul_fn(uint32_t a, uint32_t b) { return a * b; }
uint32_t div_fn(uint32_t a, uint32_t b) { return a / b; }

void testfn(modulesystem_t *modulesystem);

int main()
{
  // Initialize the modulesystem
  modulesystem_t modulesystem;
  modulesystem_init(&modulesystem);

  // Create a new module
  module_t MathModule;
  module_create(&MathModule, "MathModule");

  module_bindfunction(&MathModule, MathCategory, add, add_fn);
  module_bindfunction(&MathModule, MathCategory, sub, sub_fn);
  module_bindfunction(&MathModule, MathCategory, mul, mul_fn);
  module_bindfunction(&MathModule, MathCategory, div, div_fn);

  modulesystem_addmodule(&modulesystem, &MathModule);

  testfn(&modulesystem);

  // Destroy modules
  module_destroy(&MathModule);
  // Deinitialize the modulesystem
  modulesystem_deinit(&modulesystem);
}

void testfn(modulesystem_t *modulesystem)
{
  module_t *MathModule = modulesystem_getmodule(modulesystem, "MathModule");

  // module_getfunction(...) returns NULL if function not found
  uint32_t (*addfn)() = module_getfunction(MathModule, MathCategory, add);
  uint32_t (*subfn)() = module_getfunction(MathModule, MathCategory, sub);
  uint32_t (*mulfn)() = module_getfunction(MathModule, MathCategory, mul);
  uint32_t (*divfn)() = module_getfunction(MathModule, MathCategory, div);

  uint32_t num1 = 10, num2 = 2;
  printf("Addition of %d and %d results in %d\n"      , num1, num2, addfn(num1, num2));
  printf("Subtraction of %d from %d results in %d\n"  , num2, num1, subfn(num1, num2));
  printf("Multiplication of %d and %d results in %d\n", num1, num2, mulfn(num1, num2));
  printf("Division of %d by %d results in %d\n"       , num1, num2, divfn(num1, num2));
}
