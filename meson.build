# main meson.build file
# I'm trying to avoid putting anything that refers to the contents of
# subdirectories here, and trying to avoid putting any concerns about what the.
# host machine is in the subdirectories.

project('MAR1D', 'c')

# this project makes extensive use of char subscripts, and i'm proud of it.
c_args = [ '-Wno-char-subscripts' ]

is_windows = host_machine.system() == 'windows'
static_deps = get_option('static')

cc = meson.get_compiler('c')

# find dependencies (depending on host)
math_lib = cc.find_library('m', required : false)
opengl_dep = is_windows ? cc.find_library('opengl32', required : true) : dependency('opengl', static : static_deps)
glu_dep = is_windows ? cc.find_library('glu32', required : true) : dependency('glu', static : static_deps)
sdl2_dep = dependency('sdl2', static : static_deps)
sdl2_mixer_dep = dependency('SDL2_mixer', static : static_deps)
libconfig_dep = dependency('libconfig', static : static_deps)


# libconfig needs this defined if it's static
c_args += static_deps ? ['-D LIBCONFIG_STATIC'] : []

if get_option('portable')
    resources_dir = get_option('datadir') / ''
else
    resources_dir = get_option('prefix') / get_option('datadir') / 'mar1d' / ''
endif

subdir('src')

install_subdir('resources', install_dir: resources_dir, strip_directory: true)
