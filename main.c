#include <xc.h>
#include "header.h"
Mode mode;
unsigned int u[4]={0,0,0,0};
unsigned int p[4]={0,0,0,0};
unsigned char data[9];
unsigned char ssd[4];
int SAVE = 0;
void init_config() {
    
    init_digital_keypad();
    init_can();
    TRISB=0x01;
    PORTB=0x00;
    init_external_interrupt();
    mode=disp_mode;
    init_i2c();
}
void delay(unsigned short factor) {
    unsigned short i, j;

    for (i = 0; i < factor; i++) {
        for (j = 500;j--;);
}
}
void main(void) {
    init_config();
    
    while (1) {
        if(int_on==1&&flag==0)
        {
           init_ssd();
           flag=1;
        }
        
        key = read_digital_keypad(EDGE);
        if (key == sw3 && mode != disp_mode) {
            SAVE = 1;
        }
        switch(mode)
        {
            case disp_mode:
                display_mode();
                break;
                
            case u_st:
                U_st();
                break;
                
            case p_id:
                P_id();
                break;
                
            case save_send:
                save_trans();
                break;
                
            case wait_rec:
                waiting();
                break;
                
            case rec_modify:
                rec_send();
                break;
        }
        
    }
}

void display_mode()
{
    if(key==sw3)
    {
        selection++;   
    }  
    if(selection%2==0)
       {
          ssd[0] = 0x8F; 
          if(key==sw2)
          {
              mode=p_id;
          }
       }
       else
       {
          ssd[0] = 0xE5;
          if(key==sw2)
          {
              mode=u_st;
          }
       }
    ssd[1] = 0x40;
    ssd[2] = 0x84;
    ssd[3] = 0xE9;
    display(ssd);
}

void U_st()
{
    if(key==sw2)
    {
        pos_u++;
        if(pos_u>3)
            pos_u=0;
    }
    if(key==sw1)
    {
        u[pos_u]++;
        if(u[pos_u]>9)
        {
            u[pos_u]=0;
        }
    }
    ssd[0]=digits[u[0]];
   ssd[1] = digits[u[1]];
   ssd[2] = digits[u[2]];
   ssd[3] = digits[u[3]]; 
   ssd[pos_u]|=0x10;
   display(ssd);
   
    if(key==sw3)
    {
        mode=save_send;
        mode=wait_rec;
        SAVE=0;
    }
   
   
}

void P_id()
{
    if(key==sw2)
    {
        pos_p++;
        if(pos_p>3)
            pos_p=0;
    }
    if(key==sw1)
    {
        p[pos_p]++;
        if(p[pos_p]>9)
        {
            p[pos_p]=0;
        }
    }
    ssd[0]=digits[p[0]];
   ssd[1] = digits[p[1]];
   ssd[2] = digits[p[2]];
   ssd[3] = digits[p[3]]; 
   
   ssd[pos_p]|=0x10;
   display(ssd);
    if(key==sw3)
    {
        mode=save_send;
        mode=wait_rec;
        SAVE=0; 
    }
   
}

void save_trans()
{
    unsigned char i;
    for(i=0;i<4;i++)
    {
        write_external_eeprom(0x00+i,u[i]+'0');
    }
    
    for(i=0;i<4;i++)
    {
        write_external_eeprom(0x04+i,p[i]+'0');
    }
    for(int j=0;j<4;j++)
    {
        data[i]=u[i]+'0';
    }
    for(int j=4;j<8;j++)
    {
        data[j]=p[j-4]+'0';
    }
    can_transmit();
    delay(1000);
    //PORTA = PORTA | 0x0F;      //off ssd
    PORTD = 0xFF;
}

void waiting()
{
    static int rec_flag=0;
    rec_flag=can_receive();
    if(rec_flag==1)
    {
        init_ssd();
        rec_flag=0;
        mode=rec_modify;
    }
}

void rec_send()
{
    if(key==sw1)
    {
        val++;
        if(val>1000)
        {
            val=1000;
        }
    }
    if(key==sw2)
    {
        val--;
    }
    ssd[0] = digits[val / 1000];
    ssd[1] = digits[(val / 100) % 10];
    ssd[2] = digits[(val / 10) % 10];
    ssd[3] = digits[val % 10];
    display(ssd);
    
    u[0]=val/1000;
    u[1]=(val/100)%10;
    u[2]=(val/100)%10;
    u[3]=val%10;
    if(key==sw3)
    {
        mode=save_send;
        mode=wait_rec;
        SAVE=0;
    }
}