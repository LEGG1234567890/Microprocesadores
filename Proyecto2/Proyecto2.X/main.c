#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"

#define _XTAL_FREQ 8000000

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config LVP = OFF

#define BOTON PORTBbits.RB0
#define BUZZER PORTDbits.RD3

#define LED1 PORTDbits.RD0
#define LED2 PORTDbits.RD1
#define LED3 PORTDbits.RD2

LCD lcd;

// Variables del juego
uint8_t dino_row = 1;       // 0 = arriba, 1 = suelo
uint8_t cactus = 15;
uint8_t vidas = 3;

// Control de salto no bloqueante
uint8_t salto = 0;          // 0 = en suelo, 1 = en el aire
uint8_t timer_salto = 0;

// Patrones de caracteres personalizados en HEXADECIMAL (seguro)
const uint8_t dino_pattern[8] = {
    0x00, 0x07, 0x17, 0x1F,
    0x0E, 0x08, 0x0C, 0x00
};

const uint8_t cactus_pattern[8] = {
    0x04, 0x04, 0x15, 0x0E,
    0x04, 0x1F, 0x04, 0x04
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
void leds()
{
    LED1 = (vidas >= 1);
    LED2 = (vidas >= 2);
    LED3 = (vidas >= 3);
}

// ================= CARGA DE CARACTERES PERSONALIZADOS =================
void LCD_CustomChar(uint8_t location, const uint8_t *pattern)
{
    if (location < 8)
    {
        LCD_Cmd(0x40 + (location * 8));   // Direccion de CGRAM
        for (uint8_t i = 0; i < 8; i++)
        {
            LCD_putc(pattern[i]);         // Escribe el patron en CGRAM
        }
    }
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
        beep(200);          // Sonido de muerte

        vidas--;
        cactus = 15;

        if (vidas == 0)
        {
            LCD_Clear();
            LCD_Set_Cursor(0, 1);
            LCD_putrs("   GAME OVER    ");

            LED1 = 0;
            LED2 = 0;
            LED3 = 0;
            BUZZER = 0;
            while (1);
        }
    }
}

// ================= DIBUJO =================
void draw()
{
    char line1[17], line2[17];
    uint8_t i;

    // Linea 1 (aire)
    for (i = 0; i < 16; i++) line1[i] = ' ';
    line1[16] = '\0';
    if (dino_row == 0) {
        line1[2] = 1;               // Dinosaurio en el aire (columna 2)
    }

    // Linea 2 (suelo)
    for (i = 0; i < 16; i++) line2[i] = ' ';
    line2[16] = '\0';
    if (dino_row == 1) {
        line2[2] = 1;               // Dinosaurio en el suelo (columna 2)
    }
    if (cactus < 16) {
        line2[cactus] = 2;          // Cactus (codigo 2) en su columna
    }

    LCD_Set_Cursor(0, 1);
    LCD_puts(line1);
    LCD_Set_Cursor(1, 1);
    LCD_puts(line2);
}

// ================= MAIN =================
void main()
{
    ANSEL = 0;
    ANSELH = 0;

    TRISC = 0x00;
    TRISD = 0x00;
    TRISBbits.TRISB0 = 1;

    PORTD = 0;
    BUZZER = 0;

    OPTION_REGbits.nRBPU = 0;

    lcd.PORT = &PORTC;
    lcd.RS = 2;
    lcd.EN = 3;
    lcd.D4 = 4;
    lcd.D5 = 5;
    lcd.D6 = 6;
    lcd.D7 = 7;

    LCD_Init(lcd);

    // Cargar caracteres personalizados (codigo 1 para dino, 2 para cactus)
    LCD_CustomChar(1, dino_pattern);
    LCD_CustomChar(2, cactus_pattern);

    // Pantalla de inicio
    LCD_Clear();
    LCD_Set_Cursor(0, 1);
    LCD_putrs("  DINO GAME   ");
    LCD_Set_Cursor(1, 1);
    LCD_putrs("   START...   ");
    __delay_ms(1500);

    LCD_Clear();

    while (1)
    {
        // --- Boton de salto ---
        if (BOTON == 0 && salto == 0)
        {
            __delay_ms(20);
            if (BOTON == 0)
            {
                salto = 1;
                timer_salto = 3;
                while (BOTON == 0);
            }
        }

        // --- Actualizar salto ---
        if (salto == 1)
        {
            dino_row = 0;               // Subir
            if (--timer_salto == 0)
            {
                dino_row = 1;           // Bajar
                salto = 0;
            }
        }

        update();
        leds();
        draw();

        __delay_ms(150);
    }
}