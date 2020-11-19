#ifndef CMODULAR_TEST_INTERFACES
#define CMODULAR_TEST_INTERFACES

#include <cmodular.h>

INTERFACE_NEW 
(
  MathInterface, 
  {
    PROTOTYPE(add, uint32_t, (uint32_t, uint32_t))
    PROTOTYPE(sub, uint32_t, (uint32_t, uint32_t))
    PROTOTYPE(mul, uint32_t, (uint32_t, uint32_t))
    PROTOTYPE(div, uint32_t, (uint32_t, uint32_t))
  }
)

#endif
