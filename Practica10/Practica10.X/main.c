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

volatile unsigned int tiempo = 0, contador = 0;
char exec[10];

void Timer1_Init(){
    T1CON = 0x31;      // 1:8 prescale, TMR1CS=0 (Fosc/4), TMR1ON=1
    TMR1H = 0x9E;
    TMR1L = 0x58;
    TMR1IF = 0;
    TMR1IE = 1;
    PEIE = 1;
    GIE = 1;
}

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

void __interrupt() ISR(void){
    if(TMR1IF){
        TMR1H = 0x9E;
        TMR1L = 0x58;
        contador++;
        if(contador >= 10){
            tiempo++;
            contador = 0;
        }
        TMR1IF = 0;
    }
}

void main(void){
    Timer1_Init();
    ADC_Init();
    LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7};
    LCD_Init(lcd);
    TRISD = 0b00000000;
    PORTD = 0b00000000;
    LCD_Clear();
    LCD_Set_Cursor(0, 0);
    LCD_putrs("Tiempo: 00:00");
    while(1){
        LCD_Set_Cursor(0, 8);
        sprintf(exec, "%02u:%02u", tiempo/60, tiempo%60);
        LCD_putrs(exec);
    }
}