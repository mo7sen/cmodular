#ifndef CMODULES_MODULESYSTEM_HEADER
#define CMODULES_MODULESYSTEM_HEADER

#include <module.h>
#include <hashmap.h>

typedef struct modulesystem
{
  struct hashmap *modules;
  struct hashmap *categories;
} modulesystem_t;

int32_t modulesystem_init(modulesystem_t *modulesystem);
void modulesystem_deinit(modulesystem_t *modulesystem);

int32_t modulesystem_addmodule(modulesystem_t *modulesystem, module_t *module);

module_t *modulesystem_getmodule(modulesystem_t *modulesystem, const string_t query);
#define modulesystem_hasmodule(system, module) modulesystem_getmodule(system, module)

module_t *modulesystem_getcategory(modulesystem_t *modulesystem, const string_t categoryname);
bool modulesystem_hascategory(modulesystem_t *modulesystem, const string_t categoryname);

int32_t modulesystem_start(modulesystem_t *modulesystem);

#endif
