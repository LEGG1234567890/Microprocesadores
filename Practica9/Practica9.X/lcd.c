#include <xc.h>         // Biblioteca principal del compilador XC8
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lcd.h"

//=============================================================================
// CONFIGURACI?N DE BITS DE CONFIGURACI?N (FUSES)
//=============================================================================

// Selecci?n de oscilador (usar XT si est?s usando un cristal de 4 MHz)
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (disabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection (disabled)
#pragma config WRT = OFF        // Flash Program Memory Write Enable (disabled)
#pragma config CP = OFF         // Flash Program Memory Code Protection (disabled)

//=============================================================================
// DEFINICIONES
//=============================================================================

#define _XTAL_FREQ 8000000      // Frecuencia del oscilador (para __delay_ms y __delay_us)

unsigned char lado=0, tiempo=0;   //200 - 400

void ADC_Init(){
    ANSEL = 0x03;
    ANSELH = 0;
    ADCON0 = 0x01;
    ADCON1 = 0x81;
}

unsigned int ADC_Read(unsigned char channel){
    __delay_us(5);
    ADCON0 &= 0x83;
    ADCON0 |= channel<<2;
    __delay_ms(2);
    GO_nDONE = 1;
    while(GO_nDONE);
    return ((ADRESH<<8)+ADRESL);
}

const unsigned char triste[8] = {
    0b00000,
    0b01010,
    0b01010,
    0b00000,
    0b01110,
    0b10001,
    0b00000,
    0b00000
};

const unsigned char feliz[8] = {
    0b00000,
    0b01010,
    0b01010,
    0b00000,
    0b10001,
    0b01110,
    0b00000,
    0b00000
};

void LCD_CreateChar(unsigned char pos, const unsigned char *pattern){
    LCD_Cmd(0x40 + (pos * 8));   // Ir a la CGRAM
    for(unsigned char i = 0; i < 8; i++){
        LCD_putc(pattern[i]);    // Copiar los 8 bytes
    }
    LCD_Cmd(0x80);               // Regresar a memoria normal
}

void main(void){
    ADC_Init();
    LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7};
    LCD_Init(lcd);
    LCD_CreateChar(0, triste);
    LCD_CreateChar(1, feliz);
    TRISD = 0b00000000;
    TRISC = 0;
    TRISB = 0x01;
    PORTD = 0b00000000;
    PORTC = 0;
    OPTION_REG = OPTION_REG & 0b01111111;
    INTEDG = 0;
    INTF = 0;
    INTE = 1;
    GIE = 1;
    int x=0, y=0;
    while(1){
        LCD_Clear();
        unsigned int adc_x = ADC_Read(0), adc_y = ADC_Read(1);
        if(adc_y<500){
            y=0;
        }else if(adc_y>524){
            y=1;
        }
        if(adc_x<256){
            x--;
            tiempo=1;
        }else if((255<adc_x)&&(adc_x<500)){
            x--;
            tiempo=0;
        }else if((524<adc_x)&&(adc_x<768)){
            x++;
            tiempo=0;
        }else if(adc_x>767){
            x++;
            tiempo=1;
        }
        if(x > 15) x = 0;
        if(x < 0) x = 15;
        LCD_Set_Cursor(y, x);
        if(lado){
            LCD_putc(0);
        }else{
            LCD_putc(1);
        }
        if(tiempo){
            __delay_ms(250);
        }else{
            __delay_ms(500);
        }
    }
}

void __interrupt() ISR(void){
    if(INTF){
        GIE = 0;
        lado = !lado;
        GIE = 1;
        INTF = 0;
    }
}