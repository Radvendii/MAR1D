#include <GLFW/glfw3.h>
#include "helpers.h"
#include "objects.h"
#include "rendering.h"
#include "gamelogic.h"
#include "controls.h"
#include "mechanics.h"
#include "graphics.h"
#include "windowing.h"
#include "parsing.h"
#include <time.h>
//TODO: s/GUI/HUD/g
struct state s;

int main(void){
    ob_init();
    gl_init();
    wn_init();
    gr_init();

    glfwSetTime(0.0);
    srand(time(NULL));

    while(!wn_shouldClose()) {
        if(s.menu){
            wn_menuWindow();
            gr_clear();
            gr_drawMenu();
            wn_update();
        }
        else{
            gl_update();
            gr_update();
            wn_perspWindow();
            gr_clear();
            gr_drawPersp();
            wn_dimWindow();
            gr_clear();
            gr_drawDim();
            wn_guiWindow();
            gr_drawGui();
            wn_update();
        }
    }
    gr_deinit();
    wn_deinit();
    gl_deinit();
    ob_deinit();
    exit(EXIT_SUCCESS);

    return 0;
}
