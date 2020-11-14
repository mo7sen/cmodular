#ifndef CMODULAR_HEADER
#define CMODULAR_HEADER

#include <module.h>
#include <interface.h>
#include <modulesystem.h>

#define MODULE(name) &MODULE_NAME(name)

#define MODULE_NAME(name) name##_cmodule_id

#define MODULE_NEW(name) \
  module_t MODULE_NAME(name) = module_create(#name)

#define MODULE_DEL(name) module_destroy(MODULE_NAME(name))

#define MODULE_ADDCATEGORY(module, categoryname, interface) \
  module_addcategory(module, &(modulecategory_t) { \
    .name = categoryname, \
    .vtable = interface \
  })

#endif
