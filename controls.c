#include "controls.h"
#include "helpers.h"
#include "objects.h"

void cl_forward(struct world *w){
    (*w).camX += 1;
}
