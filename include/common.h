#ifndef CMODULAR_COMMON_HEADER
#define CMODULAR_COMMON_HEADER

#ifdef CMOD_DEBUG
#include <stdio.h>
#define cmod_err(fmt, ...) fprintf(stderr, fmt"\n", __VA_ARGS__)
#else
#define cmod_err(...)
#endif

#define member_size_p(type, member) sizeof( *((type *)0)->member )

typedef char * string_t;

enum CModErrorCodes
{
  CMOD_SUCCESS,
  CMOD_OOM,
  CMOD_ERR_ADDMODULE_NAMECOLLISION,
  CMOD_ERR_ADDMODULE_DEPENDENCYNOTFOUND,
  CMOD_ERR_INIT,
  CMOD_ERR_RUN,
  CMOD_ERR_DEINIT,
};

#endif
