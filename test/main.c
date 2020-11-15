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

uint32_t add_fn(uint32_t a, uint32_t b)
{
  return a + b;
}
uint32_t sub_fn(uint32_t a, uint32_t b)
{
  return a - b;
}

int main()
{
  int32_t result;

  // Initialize the modulesystem
  modulesystem_t modulesystem;
  result = modulesystem_init(&modulesystem);
  if(result) goto exit;

  // Create a new module
  module_t AdditionModule;
  result = module_create(&AdditionModule, "AdditionModule");
  if(result) goto deinit_modulesystem;


  // Create an AdditionInterfaceInstance from the AdditionInterface
  INTERFACE(AdditionInterface) additionInstance;
  // Bind the test_add function to the interface's add function
  INTERFACE_BIND(additionInstance, add, add_fn);
  INTERFACE_BIND(additionInstance, sub, sub_fn);

  // The interface instance is attached to the module as an implementation to
  // all methods that should be implemented by "addition" modules
  result = module_addcategory(&AdditionModule, "addition", &additionInstance);
  if(result) {}

  result = modulesystem_addmodule(&modulesystem, &AdditionModule);
  if(result) {}

  // Retrieve interface instances from the modulesystem
  module_t *adderModule = modulesystem_getmodule(&modulesystem, "AdditionModule");
  INTERFACE(AdditionInterface) *adder = module_getinterface(adderModule, "addition");

  uint32_t num1 = 10, num2 = 2;
  if(adder)
  {
    printf("Addition of %d and %d results in %d\n", num1, num2, adder->add(num1, num2));
    printf("Subtraction of %d from %d results in %d\n", num2, num1, adder->sub(num1, num2));
  }

  // Destroy modules
  module_destroy(&AdditionModule);
  // Deinitialize the modulesystem
deinit_modulesystem:
  modulesystem_deinit(&modulesystem);
exit:
  return result;
}
