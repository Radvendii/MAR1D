# main meson.build file
# I'm trying to avoid putting anything that refers to the contents of
# subdirectories here, and trying to avoid putting any concerns about what the.
# host machine is in the subdirectories.

project('MAR1D', 'c')

# make sure we're nice and clean
add_project_arguments('-Werror',  language: 'c')

# this project makes extensive use of char subscripts, and i'm proud of it.
add_project_arguments( '-Wno-char-subscripts', language: 'c')

is_windows = host_machine.system() == 'windows'
is_linux = host_machine.system() == 'linux'
is_darwin = host_machine.system() == 'darwin'

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
if static_deps
    add_project_arguments('-D LIBCONFIG_STATIC', language: 'c')
endif

if get_option('portable')
    resources_dir = get_option('datadir') / ''
    executable = get_option('bindir') / 'MAR1D'
else
    resources_dir = get_option('prefix') / get_option('datadir') / 'mar1d' / ''
    executable = get_option('prefix') / get_option('bindir') / 'MAR1D'
endif

subdir('src')

install_subdir('resources', install_dir: resources_dir, strip_directory: true)

if is_linux
   configure_file(input: 'accoutrements/mar1d.desktop.in',
                  output: 'mar1d.desktop',
                  install_dir: 'share' / 'applications',
                  configuration: { 'EXECUTABLE': executable
                                 , 'ICON' : 'MAR1D'
                                 })
  install_data('accoutrements/MAR1D.png', install_dir: 'share' / 'icons' / 'hicolor' / '256x256' / 'apps')
endif
