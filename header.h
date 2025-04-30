/* 
 * File:   client_header.h
 * Author: DELL
 *
 * Created on March 20, 2025, 4:25 PM
 */

#ifndef HEADER_H
#define	HEADER_H

#include <xc.h>
#include "external_eeprom.h"
#include "can.h"


#define LEVEL 0
#define EDGE 1

#define sw1 0x0E
#define sw2 0x0D
#define sw3 0x0B
#define sw4 0x07
#define ALL_RELEASED 0x0F

#define _XTAL_FREQ 20000000

/* Defines the data */
#define TRUE			1
#define FALSE			0

#define SLAVE_READ		0xD1
#define SLAVE_WRITE		0xD0

unsigned char digits[]={0xE7,0x21,0xCB,0x6B,0x2D,0x6E,0xEE,0x23,0xEF,0x6F};

typedef enum {
    disp_mode,u_st,p_id,save_send,wait_rec,rec_modify
} Mode;


extern Mode mode; 

void init_ssd();
void display(unsigned char *ssd);

void init_digital_keypad();
unsigned char read_digital_keypad(unsigned char detection);

void __interrupt() isr();

void init_external_interrupt();
void init_config();
void display_mode();
void P_id();
void U_st();
void save_trans();
void rec_send();

int int_on=0,flag=0;
unsigned char key;
int selection=1;
unsigned char ssd[4];
int pos_u=0,pos_p=0;
int p_ind=0,u_ind=0;

void waiting();
extern unsigned int val;



#endif	/* HEADER_H */

