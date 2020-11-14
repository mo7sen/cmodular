#ifndef CMODULES_MODULE_HEADER
#define CMODULES_MODULE_HEADER

#include <vec.h>
#include <stdbool.h>
#include <stdint.h>
#include <hashmap.h>

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
  vec_str_t category_dependencies;
  vec_str_t module_dependencies;
} module_metadata_t;

typedef struct module
{
  module_metadata_t metadata;
  struct hashmap *categories;
} module_t;
typedef vec_t(module_t) vec_module_t;

module_t module_create(const string_t name);
void module_destroy(module_t module);

void module_addcategory(module_t *module, modulecategory_t *category);
void module_adddependency(module_t *module, const string_t dependency_name, bool moduledependency);

modulecategory_t *module_getcategory(module_t *module, const string_t category_name);
void *module_getinterface(module_t *module, const string_t interface_name);
#define module_hascategory(module, name) (bool)module_getcategory(module, name);

int module_cmp(const void *mod1, const void *mod2, void *udata);
int modulecategory_cmp(const void *cat1, const void *cat2, void *udata);

uint64_t module_hash(const void *mod, uint64_t seed0, uint64_t seed1);
uint64_t modulecategory_hash(const void *cat, uint64_t seed0, uint64_t seed1);

#endif
