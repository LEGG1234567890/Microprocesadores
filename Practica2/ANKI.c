#include <xc.h>         // Biblioteca principal del compilador XC8

//=============================================================================
// CONFIGURACI?N DE BITS DE CONFIGURACI?N (FUSES)
//=============================================================================

// Selecci?n de oscilador (usar XT si est?s usando un cristal de 4 MHz)
#pragma config FOSC = HS        // Oscillator Selection bits (XT oscillator)
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

unsigned char Ana[8] = {0b11000000, 0b00110000, 0b00111100, 0b00110011, 0b00110011, 0b00111100, 0b00110000, 0b11000000};
unsigned char aNa[8] = {0b11111111, 0b11111111, 0b01110000, 0b00111000, 0b00011100, 0b00001110, 0b11111111, 0b11111111};
unsigned char Kiki[8] = {0b11000000, 0b11100000, 0b01110001, 0b00111011, 0b00011110, 0b00001100, 0b11111111, 0b11111111};
unsigned char kIki[8] = {0b11000011, 0b11000011, 0b11000011, 0b11111111, 0b11111111, 0b11000011, 0b11000011, 0b11000011};

void main(void){
    TRISB = 0;              //Filas
    TRISD = 0b00000000;     //Columnas
    PORTB = 0;              //Resetear filas
    PORTD = 0b00000000;     //Resetear columnas
    while(1){
        for (int a=0; a<201; a++){
            for(char i = 0; i < 8; i++){
                PORTB= 1 << i;
                PORTD= ~Ana[i];
                __delay_ms(5);
            }
        }
        for (int n=0; n<201; n++){
            for(char i = 0; i < 8; i++){
                PORTB= 1 << i;
                PORTD= ~aNa[i];
                __delay_ms(5);
            }
        }
        for (int k=0; k<201; k++){
            for(char i = 0; i < 8; i++){
                PORTB= 1 << i;
                PORTD= ~Kiki[i];
                __delay_ms(5);
            }
        }
        for (int j=0; j<201; j++){
            for(char i = 0; i < 8; i++){
                PORTB= 1 << i;
                PORTD= ~kIki[i];
                __delay_ms(5);
            }
        }
    }
}