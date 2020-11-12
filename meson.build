project('cmodules', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

cmodules_src = [
  'src/module.c',
  'src/modulesystem.c',
]

cmodules_inc = [
  'include',
]

vec_dep = dependency('vec')

cmodules_deps = [
  vec_dep,
]

cmodules_lib = library (
  'cmodules',
  cmodules_src,
  include_directories: cmodules_inc,
  dependencies: cmodules_deps,
  install : true
)

cmodules_dep = declare_dependency(
  link_with: cmodules_lib,
  include_directories: cmodules_inc,
  dependencies: cmodules_deps,
)

base = executable(
  'main',
  'test/main.c',
  dependencies: [ cmodules_dep ]
)

test('main', base)
