#ifndef CMODULES_INTERFACE_HEADER
#define CMODULES_INTERFACE_HEADER
#include <stdint.h>

#define INTERFACE(name) name##_interface_t

#define PROTOTYPE(name, returntype, ...) \
  returntype (*name) __VA_ARGS__;

#define INTERFACE_NEW(name, ...) \
  typedef struct name##_interface \
    __VA_ARGS__ \
  INTERFACE(name);

#define INTERFACE_BIND(name, fn_a, fn_b) name.fn_a = fn_b

INTERFACE_NEW
(
  BaseInterface,
  {
    PROTOTYPE(init,   uint32_t, (void))
    PROTOTYPE(deinit, uint32_t, (void))
    PROTOTYPE(run,    uint32_t, (void))
  }
)

#endif
