#include <stdio.h>
#include <stdint.h>

#include <cmodular.h>
#include "ev_interfaces.h"


uint32_t test_add(uint32_t a, uint32_t b)
{
  return a + b;
}
uint32_t test_sub(uint32_t a, uint32_t b)
{
  return a - b;
}
uint32_t test_mul(uint32_t a, uint32_t b)
{
  return a * b;
}
uint32_t test_div(uint32_t a, uint32_t b)
{
  return a / b;
}

int main()
{
  modulesystem_t modulesystem;
  // Initialize the modulesystem
  modulesystem_init(&modulesystem);

  // Create a new module
  MODULE_NEW(addition_module);

  // Create an AdditionInterfaceInstance from the AdditionInterface
  INTERFACE_IMPL(AdditionInterface, AdditionInterfaceInstance);
  // Bind the test_add function to the interface's add function
  INTERFACE_BIND(AdditionInterfaceInstance, add, test_add);
  INTERFACE_BIND(AdditionInterfaceInstance, sub, test_sub);

  // The interface instance is attached to the module as an implementation to
  // all methods that should be implemented by "addition" modules
  MODULE_ADDCATEGORY(MODULE(addition_module), "addition", &AdditionInterfaceInstance);

  MODULE_NEW(multiplication_module);
  INTERFACE_IMPL(MultiplicationInterface, MultiplicationInterfaceInstance);
  INTERFACE_BIND(MultiplicationInterfaceInstance, mul, test_mul);
  INTERFACE_BIND(MultiplicationInterfaceInstance, div, test_div);
  MODULE_ADDCATEGORY(MODULE(multiplication_module), "multiplication", &MultiplicationInterfaceInstance);

  modulesystem_addmodule(&modulesystem, MODULE(addition_module));
  modulesystem_addmodule(&modulesystem, MODULE(multiplication_module));

  // Retrieve interface instances from the modulesystem
  INTERFACE(AdditionInterface) *addition_interface             = modulesystem_getinterface(&modulesystem, "addition");
  INTERFACE(MultiplicationInterface) *multiplication_interface = modulesystem_getinterface(&modulesystem, "multiplication");

  uint32_t num1 = 10, num2 = 2;
  if(addition_interface)
  {
    printf("Addition of %d and %d results in %d\n", num1, num2, addition_interface->add(num1, num2));
    printf("Subtraction of %d from %d results in %d\n", num2, num1, addition_interface->sub(num1, num2));
  }

  if(multiplication_interface)
  {
    printf("Multiplication of %d and %d results in %d\n", num1, num2, multiplication_interface->mul(num1, num2));
    printf("Dividing of %d by %d results in %d\n", num1, num2, multiplication_interface->div(num1, num2));
  }

  // Destroy modules
  MODULE_DEL(addition_module);
  MODULE_DEL(multiplication_module);
  // Deinitialize the modulesystem
  modulesystem_deinit(&modulesystem);
  return 0;
}
