# cmodular

## Warning
This library is highly unstable. Use at your own risk.

### List of known problems:
- No way to remove a module from the modulesystem
- Adding modules in wrong order (violating dependencies) triggers an assert.
- There are no unit tests. The tests directory contains the example used in here

## Overview

`libcmodular`'s main purpose is to work as a foundation for C projects that needa modular architecture.

### Main Features

- Flexible
- Scalable
- Can handle dependencies between modules
- Module categories, where each category has its own v-table

### API Usage

** While some operations could be done outside of a scope, I settled, for the
sake of consistency, on making them require a scope. The only exception to that
is the interface creation process. This is because, naturally, the interfaces
often need to be global. **

First, interface creation:
```c
INTERFACE_NEW
(
  AdditionInterface, // The name of the interface
  {
    // Functions that are implemented in the interface
    // Usage: PROTOTYPE(function name, return type, parameters taken)
    PROTOTYPE(add, uint32_t, (uint32_t, uint32_t))
    PROTOTYPE(sub, uint32_t, (uint32_t, uint32_t))
  }
)
```

Interface instantiation (per module):
```c
uint32_t add_fn(uint32 a, uint32_t b)
{
  return a + b;
}
uint32_t sub_fn(uint32 a, uint32_t b)
{
  return a - b;
}

// Usage: INTERFACE_IMPL(interface type, instance name)
INTERFACE(AdditionInterface) additionInstance;
INTERFACE_BIND(additionInstance, add, add_fn);
INTERFACE_BIND(additionInstance, sub, sub_fn);
```

Module creation/destruction:
```c
module_t AdditionModule;
module_create(&AdditionModule, "AdditionModule");
module_addcategory(&AdditionModule, "addition", &additionInstance);

module_destroy(&AdditionModule);
```

Modulesystem:
```c
modulesystem_t modulesystem;
modulesystem_init(&modulesystem);

modulesystem_addmodule(&modulesystem, &AdditionModule);

```

Retrieving an interface from the modulesystem:
```c
// Works well if there's only one module that implements this interface. If the
// interface is implemented by multiple modules, then there are no guarantees on
// which one will be returned.
INTERFACE(AdditionInterface) *adder = modulesystem_getinterface(&modulesystem, "addition");
```

```c
// The following approach, while slower, is more reliable if the name of the
// implementing module is known.
module_t *module = modulesystem_getmodule(&modulesystem, "AdditionModule");
INTERFACE(AdditionInterface) *adder = module_getinterface(module, "adddition");
```
It is highly recommended to cache the interfaces that will be reused as the search
process can be a bottleneck in the speed of repetitive tasks.

*Putting it all together:*
```c
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
```
Output:
```
Addition of 10 and 2 results in 12
Subtraction of 2 from 10 results in 8
```

### Usage tips
- `module_addcategory(...)` doesn't copy the interface. The application is responsible
  for keeping the passed pointer valid.
- The string passed to `module_create(...)` must be unique as the module is stored
  using the hash of this string. Having duplicate string identifiers will cause
  the older modules to be overwritten.
- Categories can be added to a module without implementing their interfaces. While
  this is mostly useless due to the inability to search for a module that has multiple
  categories, it might be useful when trying to delay the binding of the functions.
- While there aren't specific error codes, functions that can fail return 0 on success
  and otherwise on fail. *_get* functions don't follow this rule as they should return
  a value and return 0 (NULL) on fail.
- All known ways to fail are OOM (Out Of Memory) errors.

### TODO

- [ ] Write tests
- [ ] Set up CI
- [x] Design a consistent API
