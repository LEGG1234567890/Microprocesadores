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

unsigned char lado=1;
const unsigned char carita[8] = {
    0b00000,
    0b01010,
    0b01010,
    0b00000,
    0b10001,
    0b01110,
    0b00000,
    0b00000
};

const unsigned char casita[8] = {
    0b00100,
    0b01110,
    0b11111,
    0b10001,
    0b10001,
    0b10001,
    0b11111,
    0b00000
};

void LCD_CreateChar(unsigned char pos, const unsigned char *pattern)
{
    LCD_Cmd(0x40 + (pos * 8));   // Ir a la CGRAM

    for(unsigned char i = 0; i < 8; i++)
    {
        LCD_putc(pattern[i]);    // Copiar los 8 bytes
    }

    LCD_Cmd(0x80);               // Regresar a memoria normal
}

void main(void){
    ANSEL  = 0;
    ANSELH = 0;
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
    LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7};
    LCD_Init(lcd);
    LCD_CreateChar(0, carita);
    LCD_CreateChar(1, casita);
    while(1){
        LCD_Clear();
        LCD_Set_Cursor(0, 0);
        if(lado){
            LCD_putrs("Hola mundo!");
            LCD_Set_Cursor(1, 0);
            LCD_putc(0);     // Mostrar la carita
            __delay_ms(300);
        }else{
            LCD_putrs("Adios mundo!");
            LCD_Set_Cursor(1, 0);
            LCD_putc(1);     // Mostrar la casita
        }
        __delay_ms(1000);
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