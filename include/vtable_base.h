#ifndef CMODULES_VTABLE_BASE_HEADER
#define CMODULES_VTABLE_BASE_HEADER

#include <vtable.h>

#include <stdint.h>

#define VTABLE_ENTRY(name, returntype, ...) \
  returntype (*name) (__VA_ARGS__);

#define VTABLE(name, ...) \
  typedef struct name##_vtable \
    __VA_ARGS__ \
  VTABLE_NAME(name);

VTABLE
(
  base,
  {
    VTABLE_ENTRY(init  , uint32_t, void)
    VTABLE_ENTRY(deinit, uint32_t, void)
  }
)

#endif
