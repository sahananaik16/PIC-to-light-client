#include "header.h"

void init_digital_keypad()
{
    TRISC=TRISC|0X0F;
}

unsigned char read_digital_keypad(unsigned char detection)
{
    static unsigned char once=1;
    if(detection==0)
    {
        return (PORTC & 0x0F);
    }
    else if(detection==1)
    {
        if((PORTC & 0x0F)!=0x0F && once )
        {
            once=0;
            return (PORTC & 0x0F);
        }
        else if((PORTC & 0x0F)==0x0F)
        {
            once=1;
        }
        return 0x0F;
    }
}


