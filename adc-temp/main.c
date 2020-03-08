/*
 * File:   temp.c
 * Author: Aleksandr Valentirov
 *
 * Created on March 5, 2020, 10:07 PM
 */


#include <xc.h>
#include <stdbool.h>
#include "config.h"
#include "uart.h"

#define _XTAL_FREQ 4000000
#define _DESIRED_TIME_SEC 1
#define _T0_START (short)(65536 - _DESIRED_TIME_SEC * _XTAL_FREQ / (4 * 16))
#define _DESIRED_BAUD_RATE 9600
#define _BAUD_RATE (short)(_XTAL_FREQ /_DESIRED_BAUD_RATE/16 - 1)

void T0_init(void);
void adc_init(void);
void EUSART_init(void);

short tmp;
bool check = false;

void main(void) {
    TRISA3 = 0;
    LATA3 = 0;
    
    tmp = 0;
    
    OSCCON1 = 0;  // clock source
    OSCFRQ = 0b00000010;  // 3 lsbs set clock frequency
    GIE = 1;
    PEIE = 1;
    
    T0_init();
    adc_init();
    EUSART_init();
    
    T0EN = 1;
    
    while (1){
        if (check){
            char t[4];
            int i = 3;
            while (i > -1){
                t[i] = tmp % 10 + 48;
                tmp /= 10;
                i--;
            }
            uart_send_ascii(t, 4);
        }
        check = false;
    }
    return;
}

void __interrupt() ISR (void){
    if (TMR0IE && TMR0IF){
        LATA3 = ~LATA3;
        ADCON0bits.ADGO = 1;
        TMR0 = _T0_START;
        TMR0IF = 0;
        
        T0EN = 0;
    }
    if (ADIF && ADIE){
        tmp = ADRES;
        ADIF = 0;
        T0EN = 1;
        check = true;
    }
    return;
}

void adc_init(void){
    TRISAbits.TRISA4 = 1;
    ANSELAbits.ANSA4 = 1; 
    ADIF = 0;
    ADIE = 1;
    
    ADCLK = 1; // Fosc / 4

    ADREFbits.ADNREF = 0; // Vref- is Vss
    ADREFbits.ADPREF = 0; // Vref+ is Vdd

    ADPCH = 0b000100; // channel ANA4
    
    ADCON3bits.ADTMD = 3;  // when set the ADIF
    
    ADFRM0 = 1; // result is right-justified
//    ADCON2bits.ADMD = 0; // Basic mode
    ADCON0bits.ADON = 1;
    __delay_us(10);  // pre-charge    
}

void T0_init(void){
    T016BIT = 1;
    T0CON1bits.T0CS = 2;
    T0CON1bits.T0CKPS = 4;
    TMR0 = _T0_START;
    TMR0IF = 0;
    TMR0IE = 1;
}

void EUSART_init(void){
    
    TX1STAbits.BRGH = 1;  // HIGH-SPEED mode
    TX1STAbits.SYNC = 0;  // ASYNC mode
    SP1BRG = _BAUD_RATE;
    
    RC1STAbits.SPEN = 1;  // enable serial port
    TX1STAbits.TXEN = 1;  // ENABLE transmit
    RC1STAbits.CREN = 1;  // ENABLE continuous receive
    
    RC6PPS = 0x10;
    
    ANSELCbits.ANSC7 = 0;
    ANSELCbits.ANSC6 = 0;
    TRISC6 = 1;
    TRISC7 = 1;
    
    TXIE = 0;
}