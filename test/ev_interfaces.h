#ifndef EV_VTABLES_HEADER
#define EV_VTABLES_HEADER

#include <cmodular.h>
#include <stdint.h>

INTERFACE_NEW 
(
  AdditionInterface, 
  {
    PROTOTYPE(add, uint32_t, (uint32_t, uint32_t))
    PROTOTYPE(sub, uint32_t, (uint32_t, uint32_t))
  }
)

INTERFACE_NEW 
(
  MultiplicationInterface,
  {
    PROTOTYPE(mul, uint32_t, (uint32_t, uint32_t))
    PROTOTYPE(div, uint32_t, (uint32_t, uint32_t))
  }
)

#endif
