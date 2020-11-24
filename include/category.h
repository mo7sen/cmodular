#ifndef CMODULES_CATEGORY_HEADER
#define CMODULES_CATEGORY_HEADER
#include <common.h>
#include <interface.h>

#define CATEGORY(catname) catname##_category_type

#define CATEGORY_DEFINE(catname, intname) \
  typedef struct \
  { \
    string_t name; \
    INTERFACE(intname) *vtable; \
  } CATEGORY(catname);

CATEGORY_DEFINE(BaseCategory, BaseInterface)

#endif
