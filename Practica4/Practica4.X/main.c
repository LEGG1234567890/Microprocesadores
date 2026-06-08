#include <xc.h>         // Biblioteca principal del compilador XC8

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

unsigned char Estado1, Estado2, Estado3;
unsigned char Estado1A = 0, Estado2A = 0, Estado3A = 0;
unsigned char Estado = 0;
unsigned char patron[10] = {0x3f, 0x6, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x7, 0x7f, 0x6f, 0x77};
int cuenta = 0;

void main(void){
    ANSEL  = 0;
    ANSELH = 0;
    
    // Pull-ups internos activados
    OPTION_REG = OPTION_REG & 0b01111111;
    
    TRISB = 0xFF;   // Puerto B como entrada (botones)
    TRISC = 0x00;   // Puerto C como salida (display decenas)
    TRISD = 0x00;   // Puerto D como salida (display unidades)
    
    PORTC = 0x00;
    PORTD = 0x00;

    while(1){
        Estado1 = (PORTBbits.RB0 == 0) ? 1 : 0;  // Botón suma
        Estado2 = (PORTBbits.RB1 == 0) ? 1 : 0;  // Botón resta
        Estado3 = (PORTBbits.RB2 == 0) ? 1 : 0;  // Botón modo
        if(Estado1 == 1 && Estado1A == 0){
            if(Estado == 0)
                cuenta += 1;
            else
                cuenta += 2;
        }
        if(Estado2 == 1 && Estado2A == 0){
            if(Estado == 0)
                cuenta -= 1;
            else
                cuenta -= 2;
        }
        if(Estado3 == 1 && Estado3A == 0){
            Estado = !Estado;
        }
        Estado1A = Estado1;
        Estado2A = Estado2;
        Estado3A = Estado3;
        if(cuenta > 99) cuenta = cuenta - 100;
        if(cuenta < 0)  cuenta = cuenta + 100;
        PORTD = patron[cuenta % 10];        // Unidades ? display derecho
        PORTC = patron[(cuenta / 10) % 10]; // Decenas ? display izquierdo
    }
}