#include <stdio.h>
#include <stdint.h>

typedef enum
{
  RENDERING,
  PHYSICS,

  CATEGORY_COUNT,
} CategoriesList;

typedef uint32_t CategoryType;

typedef struct
{
  void (*init)();
  void (*deinit)();
} vtable;

typedef struct
{
  void (*draw)();
} rendering_vtable;

typedef struct
{
  void (*step)();
} physics_vtable;

typedef struct
{
  CategoryType type;
  vtable *vt;
} Category;

typedef struct
{
  const char *name;

  uint8_t categoryCount;
  uint8_t categoryIdx[CATEGORY_COUNT];

  Category categories;

} Metadata;

typedef struct
{
  Metadata meta;
} Module;

#define CREATE_MODULE(modulename) \
  Module modulename = { \
    .meta = { \
      .name = #modulename, \
    } \
  }; \

#define MODULE_HAS_CATEGORY(modulename, category)

int main()
{
  CREATE_MODULE(evol_renderer);

  printf("evol_renderer.meta.name = %s\n", evol_renderer.meta.name);

  return 0;
}
