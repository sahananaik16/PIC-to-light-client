#include "header.h"

void init_external_interrupt() {
    GIE=1;
    PEIE=1;
    INT0IE=1;
    INT0IF=0;
    INTEDG0=1;
}
