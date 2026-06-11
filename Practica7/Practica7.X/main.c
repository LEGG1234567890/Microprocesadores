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

unsigned int lado=0;

void ADC_Init(){
    ANSEL = 0x01;
    ADCON0 = 0x81;
    ADCON1 = 0x80;
}

unsigned int ADC_Read(){
    __delay_us(5);
    GO_nDONE = 1;
    while(GO_nDONE);
    return ((ADRESH<<8)+ADRESL);
}

void main(void){
    ADC_Init();
    LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7};
    LCD_Init(lcd);
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
    char buffer[10];
    while(1){
        LCD_Clear();
        unsigned int adc_result = ADC_Read();
        LCD_Set_Cursor(1, 0);
        LCD_putrs("Voltaje: ");
        if(lado==0){
            LCD_Set_Cursor(0, 0);
            LCD_putrs("Numerico");
            LCD_Set_Cursor(1, 9);
            unsigned int volt = adc_result * 50000 / 1023;
            unsigned int part_ent = volt/10000, part_dec = volt%10000;
            sprintf(buffer, "%u.%u", part_ent, part_dec);
            LCD_putrs(buffer);
        }else if(lado==1){
            LCD_Set_Cursor(0, 0);
            LCD_putrs("Porcentaje");
            LCD_Set_Cursor(1, 9);
            unsigned long volt = (unsigned long)adc_result * 10000UL / 1023UL;
            unsigned int part_ent = (unsigned int)(volt / 100);
            unsigned int part_dec = (unsigned int)(volt % 100);
            sprintf(buffer, "%u.%02u%%", part_ent, part_dec);
            LCD_putrs(buffer);
        }else{
            LCD_Set_Cursor(0, 0);
            LCD_putrs("ADC");
            LCD_Set_Cursor(1, 9);
            sprintf(buffer, "%u", adc_result);
            LCD_putrs(buffer);
        }
        __delay_ms(50);
    }
}

void __interrupt() ISR(void){
    if(INTF){
        GIE = 0;
        lado++;
        GIE = 1;
        INTF = 0;
        lado=lado%3;
    }
}