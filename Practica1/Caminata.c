#include <xc.h>         // Biblioteca principal del compilador XC8

//=============================================================================
// CONFIGURACI N DE BITS DE CONFIGURACI N (FUSES)
//=============================================================================

// Selecci n de oscilador (usar XT si est s usando un cristal de 4 MHz)
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

int cuenta = 0;

void main(void){
    TRISD = 0b00000000;
    PORTD = 0b00000000;
    while(1){
        cuenta++;
        switch(cuenta){
            case 1:
                PORTD = 0b00000001;
                break;
            case 2:
                PORTD = 0b00000010;
                break;
            case 3:
                PORTD = 0b00000100;
                break;
            case 4:
                PORTD = 0b00001000;
                break;
            case 5:
                PORTD = 0b00010000;
                break;
            case 6:
                PORTD = 0b00100000;
                break;
            case 7:
                PORTD = 0b01000000;
                break;
            case 8:
                PORTD = 0b10000000;
                break;
            case 9:
                PORTD = 0b01000000;
                break;
            case 10:
                PORTD = 0b00100000;
                break;
            case 11:
                PORTD = 0b00010000;
                break;
            case 12:
                PORTD = 0b00001000;
                break;
            case 13:
                PORTD = 0b00000100;
                break;
            case 14:
                PORTD = 0b00000010;
                break;
        }
        __delay_ms(500);
        cuenta=cuenta%14;
    }
}