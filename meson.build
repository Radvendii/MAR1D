# main meson.build file
# I'm trying to avoid putting anything that refers to the contents of
# subdirectories here, and trying to avoid putting any concerns about what the.
# host machine is in the subdirectories.

project('MAR1D', 'c')

# builds with static libraries and puts everything in the same directory
# so that everything can be easily zipped and transported
is_windows = host_machine.system() == 'windows'

cc = meson.get_compiler('c')

# find dependencies (depending on host)
math_lib = cc.find_library('m', required : false)
opengl_dep = is_windows ? cc.find_library('opengl32', required : true) : dependency('opengl')
glu_dep = dependency('glu', static : is_windows)
sdl2_dep = dependency('sdl2', static : is_windows)
sdl2_mixer_dep = dependency('SDL2_mixer', static : is_windows)
libconfig_dep = dependency('libconfig', static : is_windows)

# libconfig needs this defined if it's static
c_args = is_windows ? ['-D LIBCONFIG_STATIC'] : []

# set the resource directory path (for installation and for inside the code)
if is_windows
    exe_install_dir = ''
    resources_install_dir = ''
    resources_directory = 'resources' / ''
else
    exe_install_dir = 'bin'
    resources_install_dir = 'share' / 'mar1d'
    resources_directory = get_option('prefix') / 'share' / 'mar1d' / 'resources' / ''
endif

subdir('src')

install_subdir('resources', install_dir: resources_install_dir)

# for AppImage (which doesn't work)
# if not is_windows
#     configure_file(input : 'mar1d.desktop.in',
#                    output: 'mar1d.desktop',
#                    install_dir: 'share' / 'applications',
#                    configuration:  { 'EXECUTABLE' : get_option('prefix') / exe_install_dir / 'MAR1D'
#                                    , 'ICON' : 'MAR1D'
#                                    })
#     install_data('MAR1D.png', install_dir: 'share' / 'icons' / 'hicolor' / '256x256' / 'apps')
# endif
