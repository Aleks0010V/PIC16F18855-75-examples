/*
 * File:   main.c
 * Author: Aleks
 *
 * Created on May 30, 2019, 8:10 AM
 */


#include <xc.h>
#include "config.h"

int LED[4];
void led_update(int led[]);

void main(void) {
    TRISAbits.TRISA5 = 1; // RA5 for input
    ANSELAbits.ANSA5 = 0; // RA5 for digital
    T0CON0 = 0b10000000; // timer config
    T0CON1 = 0b00010000; // timer config: source - T0CKIPPS
    T0CKIPPS = 0b00000101; // timer as counter from RA5
    
    TRISAbits.TRISA0 = TRISAbits.TRISA1 = TRISAbits.TRISA2 = TRISAbits.TRISA3 = 0;
    LATAbits.LATA0 = LATAbits.LATA1 = LATAbits.LATA2 = LATAbits.LATA3 = 0;
    
    TMR0 = 0;
    for(int i = 0; i < 3; i++){
        LED[i] = 0;
    }
    while(1){
        led_update(LED);
        if(TMR0 > 15){
            TMR0 = 0;
        }
    }
    return;
}

void led_update(int led[]){
    led[0] = TMR0bits.TMR0L0;
    led[1] = TMR0bits.TMR0L1;
    led[2] = TMR0bits.TMR0L2;
    led[3] = TMR0bits.TMR0L3;
    LATAbits.LATA0 = led[3];
    LATAbits.LATA1 = led[2];
    LATAbits.LATA2 = led[1];
    LATAbits.LATA3 = led[0];
    return;
}