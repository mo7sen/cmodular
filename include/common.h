#ifndef CMODULAR_COMMON_HEADER
#define CMODULAR_COMMON_HEADER

#ifdef CMOD_DEBUG
#include <stdio.h>
#define cmod_err(fmt, ...) fprintf(stderr, fmt"\n", __VA_ARGS__)
#else
#define cmod_err(...)
#endif

enum CModErrorCodes
{
  CMOD_SUCCESS,
  CMOD_OOM,
  CMOD_ERR_ADDMODULE_NAMECOLLISION,
  CMOD_ERR_ADDMODULE_DEPENDENCYNOTFOUND,
};


#endif
