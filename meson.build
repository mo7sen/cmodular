project('cmodular', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

cmodular_src = [
  'src/module.c',
  'src/modulesystem.c',
]

cmodular_inc = [
  'include',
]

vec_dep = dependency('vec')
hashmap_dep = dependency('hashmap')
thread_dep = dependency('evol_threads')

cmodular_deps = [
  vec_dep,
  hashmap_dep,
  thread_dep,
]

cmodular_lib = library (
  'cmodular',
  cmodular_src,
  include_directories: cmodular_inc,
  dependencies: cmodular_deps,
  install : true
)

cmodular_dep = declare_dependency(
  link_with: cmodular_lib,
  include_directories: cmodular_inc,
  dependencies: cmodular_deps,
)

if meson.version().version_compare('>= 0.54.0')
  meson.override_dependency('cmodular', cmodular_dep)
endif

base = executable(
  'main',
  'test/main.c',
  dependencies: [ cmodular_dep ]
)
