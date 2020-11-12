#ifndef CMODULES_VTABLE_HEADER
#define CMODULES_VTABLE_HEADER

#define VTABLE_NAME(name) name##_vtable_t

#define VTABLE_ENTRY(name, returntype, ...) \
  returntype (*name) (__VA_ARGS__);

#define VTABLE(name, ...) \
  typedef struct name##_vtable \
    __VA_ARGS__ \
  VTABLE_NAME(name);

#endif
