#ifndef CMODULES_MODULE_HEADER
#define CMODULES_MODULE_HEADER

#include <stdint.h>
#include <vec.h>
#include <stdbool.h>
#include <assert.h>

typedef char * string_t;

typedef struct module_metadata
{
  string_t  name;
  vec_str_t categories;
  vec_str_t dependencies;
} module_metadata_t;

typedef struct module_category
{
  uint32_t uid;
  void     *vtable;
} module_category_t;

typedef struct module
{
  module_metadata_t metadata;
} module_t;

module_t module_create(const string_t name);
uint32_t module_addcategory();

typedef vec_t(module_t) vec_module_t;

#endif
