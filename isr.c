#include "header.h"

void __interrupt() isr() {
    if(INT0IF)
    {
        int_on=1;
        INT0IF=0;
    }
}
