project('vec', 'c')

vec_src = files('src/vec.c')
vec_inc = include_directories('src')

library_name_prefix = []
library_name_suffix = []

if (build_machine.system() == 'windows' and get_option('default_library') == 'static')
library_name_prefix = ''
library_name_suffix = 'lib'
endif

vec_lib = library('vec', vec_src, include_directories: vec_inc,
    name_prefix: library_name_prefix, name_suffix: library_name_suffix)

vec_dep = declare_dependency(
  link_with: vec_lib,
  include_directories: vec_inc,
)

if meson.version().version_compare('>= 0.54.0')
  meson.override_dependency('vec', vec_dep)
endif
