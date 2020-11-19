#ifndef CMODULES_CATEGORY_HEADER
#define CMODULES_CATEGORY_HEADER
#include <stdint.h>

#define CATEGORY(catname) catname##_category_type

#define CATEGORY_DEFINE(catname, intname) \
  typedef struct \
  { \
    string_t name; \
    INTERFACE(intname) *interface; \
  } CATEGORY(catname);


#endif
