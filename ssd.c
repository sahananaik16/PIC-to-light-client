/*
 * File:   ssd_013.c
 * Author: vaibh
 *
 * Created on 20 November, 2024, 6:34 PM
 */


#include "header.h"

void init_ssd() {
    ADCON1 = 0x0F;
    TRISD = 0x00;
    TRISA = TRISA & 0xF0;
    PORTA = PORTA & 0xF0;
}

void display(unsigned char *ssd) {
    for (int i = 0; i < 4; i++) {
        PORTD = ssd[i];
        PORTA = (PORTA & 0xF0) | (1 << i);
        for (unsigned int i = 1000; i--;);
    }
}
