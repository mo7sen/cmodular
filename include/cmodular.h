#ifndef CMODULAR_HEADER
#define CMODULAR_HEADER

#include <module.h>
#include <interface.h>
#include <modulesystem.h>

#define MODULE(name) name##_cmodule_id

#define MODULE_NEW(name) \
  module_t *MODULE(name) = module_create(#name)

#define MODULE_DEL(name) \
  module_destroy(MODULE(name))

#define MODULE_ADDCATEGORY(module, categoryname, interface) \
  module_addcategory(MODULE(module), &(modulecategory_t) { \
    .name = categoryname, \
    .vtable = interface \
  })

#define MODULE_GETINTERFACE(module, interfacename) \
  module_getinterface(module, interfacename)


#define MODULE_ADDDEPENDENCY_MODULE(module, dependencyname) \
  module_adddependency(module, dependencyname, true);

#define MODULE_ADDDEPENDENCY_CATEGORY(module, dependencyname) \
  module_adddependency(module, dependencyname, false);


#define MODULESYSTEM_INIT(name) \
  modulesystem_init(&name)

#define MODULESYSTEM_DEINIT(name) \
  modulesystem_init(&name)

#define MODULESYSTEM_ADDMODULE(modulesystem_name, module_name) \
  modulesystem_addmodule(&modulesystem_name, MODULE(module_name))

#define MODULESYSTEM_GETMODULE(modulesystem_name, module_name) \
  modulesystem_getmodule(&modulesystem_name, #module_name)



#endif
