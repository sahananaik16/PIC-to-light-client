#include "header.h"

/* Global Variables */
unsigned char rec_data[13];
unsigned int val=0;
extern unsigned char data[9];
typedef enum _CanOpMode
{
	e_can_op_mode_bits    = 0xE0,			/* Use this to access opmode bits */
	e_can_op_mode_normal  = 0x00,
	e_can_op_mode_sleep   = 0x20,
	e_can_op_mode_loop    = 0x40,
	e_can_op_mode_listen  = 0x60,
	e_can_op_mode_config  = 0x80
} CanOpMode;

/* Configure the CAN Module */
void init_can(void)
{
	/* CAN_TX = RB2, CAN_RX = RB3 */
	TRISB2 = 0;								/* CAN TX */
	TRISB3 = 1;								/* CAN RX */

	/* Enter CAN module into config mode */
							 /* clear previous mode */
	  CAN_SET_OPERATION_MODE_NO_WAIT(e_can_op_mode_config);                     	/* set new mode */

	/* Wait untill desired mode is set */
	while (CANSTAT != 0x80);

	/* Enter CAN module into Mode 0 */
	ECANCON = 0x00;

	/* Initialize CAN Timing 8MHz */
	BRGCON1 = 0xE1;							/* 1110 0001, SJW=4, TQ, BRP 4 */
	BRGCON2 = 0x1B;							/* 0001 1011, SEG2PHTS 1 sampled once PS1=4TQ PropagationT 4TQ */
	BRGCON3 = 0x03;							/* 0000 0011, PS2, 4TQ */

	/*
	 * Enable Filters
	 * Filter 0
	 */
	RXFCON0 = 0x01;     

	/*
	 * Initialize Receive Filters
	 * Filter 0 = 0xFFC
	 */
	RXF0EIDH = 0x00;
	RXF0EIDL = 0x00;
	RXF0SIDH = 0x6B;
	RXF0SIDL = 0xC0;

	/* Enter CAN module into Loop back mode */
	CAN_SET_OPERATION_MODE_NO_WAIT(e_can_op_mode_normal);

	/* Set Receive Mode for buffers */
	RXB0CON = 0x00;
}

/* Check the buffers, if it have message */
unsigned char can_receive(void)
{
	unsigned char rx_msg_flag = 0;

	if (RXB0FUL) /* CheckRXB0 */
	{       
        rec_data[EIDH] = RXB0EIDH;
		rec_data[EIDL] = RXB0EIDL;
		rec_data[SIDH] = RXB0SIDH;
		rec_data[SIDL] = RXB0SIDL;
		rec_data[DLC] = RXB0DLC;
        rec_data[D0]=RXB0D0;
        rec_data[D1]=RXB0D1;
        rec_data[D2]=RXB0D2;
        rec_data[D3]=RXB0D3;
        rec_data[D4]=RXB0D4;
        rec_data[D5]=RXB0D5;
        rec_data[D6]=RXB0D6;
        rec_data[D7]=RXB0D7;

		RXB0FUL = 0;
		RXB0IF = 0;

        val=(rec_data[D0]-'0')*1000+(rec_data[D1]-'0')*100+(rec_data[D2]-'0')*10+(rec_data[D0]-'0');
		return TRUE;
	}
	else
	{
		return FALSE;
	}    
}

/* Transmit Sample Mesaage */
void can_transmit(void)
{
	TXB0EIDH = 0x00;		/* Extended Identifier */
	TXB0EIDL = 0x00;		/* Extended Identifier */

	/* 0x35E  0110 1011 110 */
	TXB0SIDH = 0x6B;		/* Standard Identifier */
	TXB0SIDL = 0xC0;		/* Standard Identifier */

	TXB0DLC = 0x08;			/* Data Length Count */
	TXB0D0 = data[0];			/* DataByte 0 */
	TXB0D1 = data[1];			/* DataByte 1 */
	TXB0D2 = data[2];			/* DataByte 2 */
	TXB0D3 = data[3];			/* DataByte 3 */
	TXB0D4 = data[4];			/* DataByte 4 */
	TXB0D5 = data[5];			/* DataByte 5 */
	TXB0D6 = data[6];			/* DataByte 6 */
	TXB0D7 = data[7];			/* DataByte 7 */

	TXB0REQ = 1;			/* Set the buffer to transmit */
}

