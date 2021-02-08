sources = files( 'audio.c'
               , 'controls.c'
               , 'enemies.c'
               , 'gamelogic.c'
               , 'graphics.c'
               , 'helpers.c'
               , 'mechanics.c'
               , 'menu.c'
               , 'objects.c'
               , 'parsing.c'
               , 'rendering.c'
               , 'resources.c'
               , 'windowing.c'
               , 'main.c'
               )

configure_file(input : 'resources.h.in',
               output: 'resources.h',
               configuration:  { 'RESOURCE_DIRECTORY' : resources_directory })

executable('MAR1D', sources,
           install : true,
           install_dir : exe_install_dir,
           # c_args : c_args,
           dependencies : [ math_lib
                          , opengl_dep
                          , glu_dep
                          , sdl2_dep
                          , sdl2_mixer_dep
                          , libconfig_dep
                          ])
