project('MAR1D', 'c')

is_windows = host_machine.system() == 'windows'

cc = meson.get_compiler('c')

math_lib = cc.find_library('m', required : false)

if is_windows
    resources_install_dir = 'bin'
    resources_directory = 'resources' / ''
    opengl_dep = cc.find_library('opengl32', required : true)
else
    resources_install_dir = 'share' / 'mar1d'
    resource_directory = get_option('prefix') / 'share' / 'mar1d' / 'resources' / ''
    opengl_dep = dependency('opengl')
endif

subdir('src')

install_subdir('resources', install_dir: resources_install_dir)
