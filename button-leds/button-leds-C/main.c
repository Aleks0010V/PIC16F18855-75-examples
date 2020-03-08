/*
 * File:   main.c
 * Author: Aleksandr Valentirov
 *
 * Created on September 21, 2019, 11:53 PM
 */


#include <xc.h>
#include "config.h"

void main(void) {
    ANSELA = 0x00;
    TRISA = 0x20;
    PORTA = 0x00;
    LATA = 0x00;
    while(1)
    {
        if(PORTAbits.RA5 == 0)
        {
            LATA = 0x0F;
        }
        else
        {
            LATA = 0x00;
        }
    }
    return;
}
