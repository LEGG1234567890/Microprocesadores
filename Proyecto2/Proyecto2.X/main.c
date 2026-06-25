#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"

#define _XTAL_FREQ 8000000

#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (disabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection (disabled)
#pragma config WRT = OFF        // Flash Program Memory Write Enable (disabled)
#pragma config CP = OFF         // Flash Program Memory Code Protection (disabled)

#define BUZZER PORTDbits.RD3
#define BOTON PORTBbits.RB0

// Variables del juego
uint8_t dino_row = 1, cactus = 15, vidas = 3, puntos=0;

// Control de salto no bloqueante
uint8_t salto = 0, timer_salto = 0;

// Patrones de caracteres personalizados en HEXADECIMAL (seguro)
const unsigned char dino_pattern[8] = {
    0x00, 0x07, 0x17, 0x1F,
    0x0E, 0x08, 0x0C, 0x00
};

const unsigned char cactus_pattern[8] = {
    0x04, 0x04, 0x15, 0x0E,
    0x04, 0x1F, 0x04, 0x04
};

const unsigned char muerte[8] = {
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
};

// ================= BUZZER (pitido a 1kHz) =================
void beep(uint16_t ms)
{
    for (uint16_t i = 0; i < ms; i++)   // Cada iteracion dura ~1 ms
    {
        BUZZER = 1;
        __delay_us(500);                // Medio periodo (500 us)
        BUZZER = 0;
        __delay_us(500);                // Medio periodo
    }
    BUZZER = 0;                         // Aseguramos que quede apagado
}

// ================= LEDS =================

// ================= CARGA DE CARACTERES PERSONALIZADOS =================
void LCD_CreateChar(unsigned char pos, const unsigned char *pattern)
{
    LCD_Cmd(0x40 + (pos * 8));   // Ir a la CGRAM

    for(unsigned char i = 0; i < 8; i++)
    {
        LCD_putc(pattern[i]);    // Copiar los 8 bytes
    }

    LCD_Cmd(0x80);               // Regresar a memoria normal
}

// ================= GAME =================
void update()
{
    if (cactus > 0) {
        cactus--;
    } else {
        cactus = 15;
    }

    // Colision: cactus en columna 2 y dinosaurio en el suelo
    if (cactus == 2 && dino_row == 1)
    {
        LCD_Clear();
        LCD_Set_Cursor(1, 2);
        LCD_putc(2);
        beep(200);          // Sonido de muerte
        vidas--;
        PORTD = PORTD >> 1;
        cactus = 15;

        if (vidas == 0)
        {
            LCD_Clear();
            LCD_Set_Cursor(0, 1);
            LCD_putrs("   GAME OVER    ");
            BUZZER = 0;
            while (1);
        }
    }else if(cactus==1){
        puntos++;
    }
}

// ================= MAIN =================
void main()
{
    ANSEL = 0;
    ANSELH = 0;

    TRISC = 0x00;
    TRISD = 0x00;
    BOTON = 1;

    PORTD = 0b00000111;
    BUZZER = 0;

    OPTION_REG = OPTION_REG & 0b01111111;
    INTEDG = 0;
    INTF = 0;
    INTE = 1;
    GIE = 1;
    
    LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7};
    LCD_Init(lcd);

    // Cargar caracteres personalizados (codigo 0 para dino, 1 para cactus)
    LCD_CreateChar(0, dino_pattern);
    LCD_CreateChar(1, cactus_pattern);
    LCD_CreateChar(2, muerte);
    char buffer[10];
    // Pantalla de inicio
    LCD_Clear();
    LCD_Set_Cursor(0, 1);
    LCD_putrs("  DINO GAME   ");
    LCD_Set_Cursor(1, 1);
    LCD_putrs("   START...   ");
    __delay_ms(1000);

    while (1)
    {
        // --- Boton de salto ---
        LCD_Clear();
        LCD_Set_Cursor(0, 4);
        LCD_putrs("Points:");
        sprintf(buffer, "%u", puntos);
        LCD_putrs(buffer);
        if (dino_row == 0) {
            LCD_Set_Cursor(0, 2);
        }else {
            LCD_Set_Cursor(1, 2);
        }
        LCD_putc(0);
        LCD_Set_Cursor(1, cactus);
        LCD_putc(1);
        if (BOTON == 0 && salto == 0)
        {
            __delay_ms(20);
            if (BOTON == 0)
            {
                salto = 1;
                timer_salto = 2;
            }
        }

        // --- Actualizar salto ---
        if (salto == 1)
        {
            timer_salto--;
            dino_row = 0;               // Subir
            if (timer_salto == 0)
            {
                dino_row = 1;           // Bajar
                salto = 0;
            }
        }
        update();
        __delay_ms(100);
        }
}