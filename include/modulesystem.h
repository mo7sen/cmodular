#ifndef CMODULES_MODULESYSTEM_HEADER
#define CMODULES_MODULESYSTEM_HEADER

#include <module.h>
#include <hashmap.h>

typedef struct modulesystem
{
  struct hashmap *modules;
  struct hashmap *categories;
} modulesystem_t;

void modulesystem_init(modulesystem_t *modulesystem);
void modulesystem_deinit(modulesystem_t *modulesystem);

bool modulesystem_hasmodule(modulesystem_t *modulesystem, const string_t modulename);
bool modulesystem_hascategory(modulesystem_t *modulesystem, const string_t categoryname);

void modulesystem_addmodule(modulesystem_t *modulesystem, module_t *module);

module_t *modulesystem_getmodule(modulesystem_t *modulesystem, const string_t query);
void  *modulesystem_getinterface(modulesystem_t *modulesystem, const string_t query);


#endif
