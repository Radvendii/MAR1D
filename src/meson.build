sources = [ 'audio.c'
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
          ]

configure_file(input : 'resources.h.in',
               output: 'resources.h',
               configuration:  { 'RESOURCE_DIRECTORY' : resources_directory })

executable('MAR1D', sources,
           install : true,
           c_args : is_windows ? ['-D LIBCONFIG_STATIC'] : [],
           dependencies : [ math_lib
                          , opengl_dep
                          , dependency('sdl2', static : is_windows)
                          , dependency('SDL2_mixer', static : is_windows)
                          , dependency('libconfig', static : is_windows)
                          ])
