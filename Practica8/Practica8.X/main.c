#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lcd.h"

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 8000000

void ADC_Init(){
    ANSEL = 0x03;
    ANSELH = 0;
    ADCON0 = 0x01;
    ADCON1 = 0x81;
}

void Interrupt_Init() {
    INTE = 1;
    INTEDG = 1;
    TRISB |= 0x03;
    RBIE = 0;           // RB1 no soporta RBIF, se usa polling
    RBIF = 0;
    PEIE = 1;
    GIE = 1;
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

unsigned int lado = 0;
unsigned char num = 0;

void main(void){
    ADC_Init();
    Interrupt_Init();
    LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7};
    LCD_Init(lcd);
    TRISD = 0b00000000;
    TRISC = 0;
    TRISB |= 0x03;
    PORTD = 0b00000000;
    PORTC = 0;
    OPTION_REG = OPTION_REG & 0b01111111;
    INTEDG = 0;
    INTF = 0;
    INTE = 1;
    GIE = 1;
    char buffer[10];
    unsigned char rb1_prev = 0;
    while(1){
        // Polling RB1 con debounce
        unsigned char rb1_now = PORTBbits.RB1;
        if(rb1_now == 1 && rb1_prev == 0){
            __delay_ms(30);
            if(PORTBbits.RB1){
                num = !num;
            }
        }
        rb1_prev = rb1_now;
        LCD_Clear();
        LCD_Set_Cursor(1, 0);
        unsigned int adc_result = ADC_Read(num);
        if(num){
            LCD_putrs("Voltaje 1: ");
        }else{
            LCD_putrs("Voltaje 2: ");
        }
        if(lado==0){
            LCD_Set_Cursor(0, 0);
            LCD_putrs("Numerico");
            LCD_Set_Cursor(1, 11);
            unsigned long volt = (unsigned long)adc_result * 50000UL / 1023UL;
            unsigned int part_ent = (unsigned int)(volt / 10000);
            unsigned int part_dec = (unsigned int)(volt % 10000);
            sprintf(buffer, "%u.%04u", part_ent, part_dec);
            LCD_putrs(buffer);
        }else if(lado==1){
            LCD_Set_Cursor(0, 0);
            LCD_putrs("Porcentaje");
            LCD_Set_Cursor(1, 11);
            unsigned long volt = (unsigned long)adc_result * 10000UL / 1023UL;
            unsigned int part_ent = (unsigned int)(volt / 100);
            unsigned int part_dec = (unsigned int)(volt % 100);
            sprintf(buffer, "%u.%02u%%", part_ent, part_dec);
            LCD_putrs(buffer);
        }else{
            LCD_Set_Cursor(0, 0);
            LCD_putrs("ADC");
            LCD_Set_Cursor(1, 11);
            sprintf(buffer, "%u", adc_result);
            LCD_putrs(buffer);
        }
        __delay_ms(100);
    }
}

void __interrupt() ISR(void) {
    if(INTF){
        lado = (lado + 1) % 3;
        INTF = 0;
    }
}