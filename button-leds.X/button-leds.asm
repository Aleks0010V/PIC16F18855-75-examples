; PIC16F18855 Configuration Bit Settings

; Assembly source line config statements

#include "p16f18855.inc"

; CONFIG1
; __config 0xFFFF
 __CONFIG _CONFIG1, _FEXTOSC_ECH & _RSTOSC_EXT1X & _CLKOUTEN_OFF & _CSWEN_ON & _FCMEN_ON
; CONFIG2
; __config 0xFFFF
 __CONFIG _CONFIG2, _MCLRE_ON & _PWRTE_OFF & _LPBOREN_OFF & _BOREN_ON & _BORV_LO & _ZCD_OFF & _PPS1WAY_ON & _STVREN_ON
; CONFIG3
; __config 0xFF9F
 __CONFIG _CONFIG3, _WDTCPS_WDTCPS_31 & _WDTE_OFF & _WDTCWS_WDTCWS_7 & _WDTCCS_SC
; CONFIG4
; __config 0xFFFF
 __CONFIG _CONFIG4, _WRT_OFF & _SCANE_available & _LVP_ON
; CONFIG5
; __config 0xFFFF
 __CONFIG _CONFIG5, _CP_OFF & _CPD_OFF


RES_VECT  CODE    0x0000            ; processor reset vector
    BANKSEL ANSELA
    CLRF ANSELA	    ; port A to digital
    BANKSEL PORTA
    CLRF PORTA	    ; clearing the input of port A
    BANKSEL TRISA
    CLRF TRISA	    ; all pins of port A to output
    BSF TRISA, 5    ; sixs pin of port A (0-7) to input
    
START:
    BANKSEL LATA
    CLRF LATA	    ; clearing the output of port A
    BANKSEL PORTA
    BTFSC PORTA, 5  ; checking if button is pressed
    GOTO START	    ; if not pressed, go to start
    BANKSEL LATA
    MOVLW 0x0F	    ; if pressed, turn on LEDs
    MOVWF LATA
    GOTO START	    ; go to start
    END


