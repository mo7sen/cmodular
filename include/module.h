#ifndef CMODULES_MODULE_HEADER
#define CMODULES_MODULE_HEADER

#include <vec.h>
#include <stdint.h>
#include <hashmap.h>
#include <common.h>

// This is a base struct with the sole purpose of representing other categories
// in functions
typedef struct modulecategory
{
  const string_t name;
  void *interface_instance;
} modulecategory_t;

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

int32_t module_create(module_t *module, const string_t name);
void module_destroy(module_t *module);

int32_t module_addcategory_impl(module_t *module, const string_t category_name);
int32_t module_adddependency(module_t *module, const string_t dependency_name, bool moduledependency);

struct modulecategory *module_getcategory(module_t *module, const string_t category_name);
#define module_hascategory(module_p, catname) (bool)module_getcategory(module_p, catname)

int32_t module_cmp(const void *mod1, const void *mod2, void *udata);
int32_t modulecategory_cmp(const void *cat1, const void *cat2, void *udata);

uint64_t module_hash(const void *mod, uint64_t seed0, uint64_t seed1);
uint64_t modulecategory_hash(const void *cat, uint64_t seed0, uint64_t seed1);

#define module_addcategory(module_p, categorytype) \
  module_addcategory_impl(module_p, #categorytype)

// If category not found, add it.
#define module_bindfunction(module_p, categorytype, catfunc, bindfunc) \
    do { \
      CATEGORY(categorytype) *category = (CATEGORY(categorytype)*)module_getcategory(module_p, #categorytype); \
      if(!category) \
      { \
        module_addcategory(module_p, categorytype); \
        category = (CATEGORY(categorytype)*)module_getcategory(module_p, #categorytype); \
      } \
      if(!category->interface) \
      { \
        category->interface = calloc(1, member_size_p(CATEGORY(categorytype), interface)); \
      } \
      category->interface->catfunc = bindfunc; \
    } while (0)


  // BUG This function has zero null checks. It is not yet obvious whether checking
  // for the category/interface's existence should be done by the application or
  // the library.
#define module_getfunction(module_p, categorytype, func) \
    ((CATEGORY(categorytype)*)module_getcategory(module_p, #categorytype))->interface->func

#endif
