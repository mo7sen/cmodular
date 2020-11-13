#ifndef CMODULES_MODULESYSTEM_HEADER
#define CMODULES_MODULESYSTEM_HEADER

#include <module.h>

typedef struct modulesystem
{
  vec_module_t modules;
  vec_str_t categories;
  vec_str_t module_names;
} modulesystem_t;

void modulesystem_init(modulesystem_t *modulesystem);
void modulesystem_deinit(modulesystem_t *modulesystem);

bool modulesystem_hasmodule(modulesystem_t *modulesystem, const string_t modulename);
bool modulesystem_hascategory(modulesystem_t *modulesystem, const string_t categoryname);

void modulesystem_addmodule(modulesystem_t *modulesystem, module_t module);

module_t *modulesystem_getmodule(modulesystem_t *modulesystem, const string_t query);


#endif
