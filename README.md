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
  MathInterface, // The name of the interface
  {
    // Functions that are implemented in the interface
    // Usage: PROTOTYPE(function name, return type, parameters taken)
    PROTOTYPE(add, uint32_t, (uint32_t, uint32_t))
    PROTOTYPE(sub, uint32_t, (uint32_t, uint32_t))
    PROTOTYPE(mul, uint32_t, (uint32_t, uint32_t))
    PROTOTYPE(div, uint32_t, (uint32_t, uint32_t))
  }
)
```

Category creation and binding with interface:
```c
CATEGORY_DEFINE(MathCategory, MathInterface)
```

Module creation/destruction:
```c
module_t MathModule;
module_create(&MathModule, "AdditionModule");

module_destroy(&AdditionModule);
```

Binding functions:
```c
  uint32_t add_fn(uint32_t a, uint32_t b) { return a + b; }
  uint32_t sub_fn(uint32_t a, uint32_t b) { return a - b; }
  uint32_t mul_fn(uint32_t a, uint32_t b) { return a * b; }
  uint32_t div_fn(uint32_t a, uint32_t b) { return a / b; }


  module_bindfunction(&MathModule, MathCategory, add, add_fn);
  module_bindfunction(&MathModule, MathCategory, sub, sub_fn);
  module_bindfunction(&MathModule, MathCategory, mul, mul_fn);
  module_bindfunction(&MathModule, MathCategory, div, div_fn);
```

Modulesystem:
```c
modulesystem_t modulesystem;
modulesystem_init(&modulesystem);

modulesystem_addmodule(&modulesystem, &AdditionModule);

modulesystem_deinit(&modulesystem);
```

Retrieving a funciton from a module:
```c
// returntype (*fnname)() = module_getfunction(...)
// NULL if function not found
uint32_t (*addfn)() = module_getfunction(MathModule, MathCategory, add);
uint32_t (*subfn)() = module_getfunction(MathModule, MathCategory, sub);
uint32_t (*mulfn)() = module_getfunction(MathModule, MathCategory, mul);
uint32_t (*divfn)() = module_getfunction(MathModule, MathCategory, div);
```

Getting module by name/category from modulesystem:
```c
  module_t *MathModule = modulesystem_getmodule(modulesystem, "MathModule");
  // OR
  module_t *MathModule = modulesystem_getcategory(modulesystem, "MathCategory");
```

*Putting it all together:*
```c
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
```
Output:
```
Addition of 10 and 2 results in 12
Subtraction of 2 from 10 results in 8
Multiplication of 10 and 2 results in 20
Division of 10 by 2 results in 5
```

### Usage tips
- The string passed to `module_create(...)` must be unique as the module is stored
  using the hash of this string. Having duplicate string identifiers will cause
  the older modules to be overwritten.
- Categories can be added to a module without implementing their functions using
  `module_addcategory(&MathModule, MathCategory)`. While this is mostly useless 
  due to the inability to search for a module that has multiple categories, it 
  might be useful when trying to delay the binding of the functions.
- While there aren't specific error codes, functions that can fail return 0 on success
  and otherwise on fail. *_get* functions don't follow this rule as they should return
  a value and return 0 (NULL) on fail.
- All known ways to fail are OOM (Out Of Memory) errors.

### TODO

- [ ] Write tests
- [ ] Set up CI
- [ ] Design a consistent API
