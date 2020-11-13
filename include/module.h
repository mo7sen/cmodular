#ifndef CMODULES_MODULE_HEADER
#define CMODULES_MODULE_HEADER

#include <vec.h>
#include <stdbool.h>

typedef char * string_t;

typedef struct modulecategory
{
  string_t name;
  void *vtable;
} modulecategory_t;
typedef vec_t(modulecategory_t) vec_category_t;

typedef struct module_metadata
{
  string_t  name;
  vec_str_t dependencies;
} module_metadata_t;

typedef struct module
{
  module_metadata_t metadata;
  vec_category_t categories;
} module_t;
typedef vec_t(module_t) vec_module_t;



module_t module_create(const string_t name);

void module_destroy(module_t module);

void module_addcategory(module_t *module, modulecategory_t category);

bool module_hascategory(module_t *module, const string_t category_name);
int module_getcategoryidx(module_t *module, const string_t category_name);

#define MODULE(name) name##_cmodule_id
#define MODULE_NEW(name) \
  module_t MODULE(name) = module_create(#name)
#define MODULE_DEL(name) module_destroy(MODULE(name))
#define MODULE_ADDCATEGORY(module, categoryname, interface) \
  module_addcategory(&module, (modulecategory_t) { \
    .name = categoryname, \
    .vtable = interface \
  })

#endif
