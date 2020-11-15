#include <stdio.h>
#include <stdint.h>

#include <cmodular.h>

INTERFACE_NEW 
(
  AdditionInterface, 
  {
    PROTOTYPE(add, uint32_t, (uint32_t, uint32_t))
    PROTOTYPE(sub, uint32_t, (uint32_t, uint32_t))
  }
)

uint32_t test_add(uint32_t a, uint32_t b)
{
  return a + b;
}
uint32_t test_sub(uint32_t a, uint32_t b)
{
  return a - b;
}

int main()
{
  // Initialize the modulesystem
  modulesystem_t modulesystem;
  MODULESYSTEM_INIT(modulesystem);

  // Create a new module
  MODULE_NEW(AdditionModule);

  // Create an AdditionInterfaceInstance from the AdditionInterface
  INTERFACE_IMPL(AdditionInterface, additionInterfaceInstance);
  // Bind the test_add function to the interface's add function
  INTERFACE_BIND(additionInterfaceInstance, add, test_add);
  INTERFACE_BIND(additionInterfaceInstance, sub, test_sub);

  // The interface instance is attached to the module as an implementation to
  // all methods that should be implemented by "addition" modules
  MODULE_ADDCATEGORY(AdditionModule, "addition", &additionInterfaceInstance);


  MODULESYSTEM_ADDMODULE(modulesystem, AdditionModule);

  // Retrieve interface instances from the modulesystem
  module_t *adderModule = MODULESYSTEM_GETMODULE(modulesystem, AdditionModule);
  INTERFACE(AdditionInterface) *adder = MODULE_GETINTERFACE(adderModule, "addition");

  uint32_t num1 = 10, num2 = 2;
  if(adder)
  {
    printf("Addition of %d and %d results in %d\n", num1, num2, adder->add(num1, num2));
    printf("Subtraction of %d from %d results in %d\n", num2, num1, adder->sub(num1, num2));
  }

  // Destroy modules
  MODULE_DEL(AdditionModule);
  // Deinitialize the modulesystem
  MODULESYSTEM_DEINIT(modulesystem);
  return 0;
}
