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

unsigned char patron[10] = {0x3f, 0x6, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x7, 0x7f, 0x6f};
unsigned char lado=1;
int num = 0;

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
    while(1){
        int uni = num%10;
        int dec = (num/10)%10;
        int cen = (num/100)%10;
        int mil = (num/1000)%10;
        for(int i = 0; i < 50; i++){
            PORTC = 0b11110111;
            PORTD = patron[mil];
            __delay_ms(1);
            PORTC = 0b11111011;
            PORTD = patron[cen];
            __delay_ms(1);
            PORTC = 0b11111101;
            PORTD = patron[dec];
            __delay_ms(1);
            PORTC = 0b11111110;
            PORTD = patron[uni];
            __delay_ms(1);
        }
        if(lado){
            num++;
        }else{
            num--;
        }
        if(num > 9999) num = num - 10000;
        if(num < 0)  num = num + 10000;
    }
}

void __interrupt() ISR(void){
    if(INTF){
        lado = !lado;
        INTF = 0;
    }
}