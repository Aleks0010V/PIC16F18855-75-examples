/*
 * File:   lcd.c
 * Author: Aleksandr Valentirov
 *
 * Created on March 8, 2020, 1:52 PM
 */

#include <stdlib.h>
#include <xc.h>

#define _XTAL_FREQ 4000000

#define LCD_PORT_DIR TRISB
#define LCD_O_DATA LATB
#define LCD_I_DATA PORTB
#define RS LATDbits.LATD5  // 1 - data, 0 - command
#define RW LATDbits.LATD6  // 1 - read, 0 - write
#define E LATDbits.LATD7   // enable signal
#define LCD_EN_DELAY 500

// prototype, need to be finished
void lcd_write_data(unsigned char data){
    E = 0;
    LCD_PORT_DIR = 0;
    LCD_O_DATA = data;
    E = 1;
    __delay_us(LCD_EN_DELAY);
    E = 0;
    __delay_us(LCD_EN_DELAY);
    return;
}

// prototype, need to be finished
void lcd_read_data(unsigned char *dest){
    LCD_PORT_DIR = 1;
    *dest = LCD_I_DATA;
    return;
}

void lcd_send_cmd(unsigned char cmd){
    RS = 0;
    RW = 0;
    lcd_write_data(cmd);
    return;
}

void lcd_send_data(unsigned char data){
    RS = 1;
    RW = 0;
    lcd_write_data(data);
}

void display_controll(char d, char c, char b){
    // assembling final command
    unsigned char cmd = (8 | (d << 2)) | (8 | (c << 1)) | (8 | b);
    lcd_send_cmd(cmd);
    __delay_us(50);
    return;
}

void lcd_init(void){
    
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD7 = 0;
    
    RS = 0;
    RW = 0;
    E = 0;
    
    __delay_ms(150);  // power-up
    lcd_send_cmd(0b00110000);  // 
    __delay_ms(6);  // first Function Set
    
    lcd_send_cmd(0b00110000);  //
    __delay_us(150);  // second 
    lcd_send_cmd(0b00110000);  //
    __delay_us(150);  // third Function Set
    
    
    lcd_send_cmd(0b00111000);  // setting screen params by default (Function Set)
    __delay_us(100);  //
    display_controll(0, 0, 0);  // setting display params for init
    __delay_us(100);
    lcd_send_cmd(1);  // clear display
    __delay_ms(3);
    lcd_send_cmd(0b00000110);  // entry mode set command
    __delay_us(100);
    
    
    display_controll(1, 0, 0);
    __delay_us(50);
    return;
}

void lcd_set_cursor(unsigned char r, unsigned char c)
{
  unsigned char Temp;
  if(r == 1) 
  {
    Temp = 0x80 + c - 1; //0x80 is used to move the cursor
    lcd_send_cmd(Temp);
  }
  if(r == 2) 
  {
    Temp = 0xC0 + c - 1;
    lcd_send_cmd(Temp);
  }
  __delay_us(80);
}

void lcd_write_ascii(unsigned char *str, size_t data_size){
    for (size_t i = 0; i < data_size; i++){
        lcd_send_data(str[i]);
    }
}