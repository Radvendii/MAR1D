sources = files( 'lodepng.c'
               , 'audio.c'
               , 'visual_sounds.c'
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
               # TODO: should really be relative to executable, not prefix (if portable)
               # https://github.com/mesonbuild/meson/pull/7908
               configuration:  { 'RESOURCE_DIRECTORY' : resources_dir })

executable('MAR1D', sources,
           install : true,
           dependencies : [ math_lib
                          , opengl_dep
                          , glu_dep
                          , sdl2_dep
                          , sdl2_mixer_dep
                          , libconfig_dep
                          ])
