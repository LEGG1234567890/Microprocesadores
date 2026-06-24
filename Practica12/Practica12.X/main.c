#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcd.h"

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 8000000

const char keymap[4][4] = {
    {'7', '8', '9', '/'},
    {'4', '5', '6', '*'},
    {'1', '2', '3', '-'},
    {'C', '0', '=', '+'}
};

void long_a_str(long num, char *buf) {
    uint8_t i = 0, j = 0;
    char temp[12];
    uint8_t negativo = 0;
    if (num < 0) {
        negativo = 1;
        num = -num;
    }
    if (num == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    while (num > 0) {
        temp[i++] = '0' + (num % 10);
        num /= 10;
    }
    if (negativo)
        temp[i++] = '-';
    for (j = 0; j < i; j++)
        buf[j] = temp[i - 1 - j];
    buf[i] = '\0';
}

char Keypad_Scan(void) {
    uint8_t fila, col;
    uint8_t lectura;
    for (fila = 0; fila < 4; fila++) {
        PORTB = 0x0F & ~(0x01 << fila);
        __delay_us(50);
        lectura = PORTB;
        for (col = 0; col < 4; col++) {
            if (!(lectura & (0x10 << col))) {
                __delay_ms(20);
                do {
                    lectura = PORTB;
                } while (!(lectura & (0x10 << col)));
                return keymap[fila][col];
            }
        }
    }
    return 0;
}

void mostrar_resultado(long resultado) {
    char res[17];
    LCD_Clear();
    __delay_ms(2);
    LCD_Set_Cursor(0, 0);
    long_a_str(resultado, res);
    LCD_puts(res);
}

void mostrar_division(long num1, long num2) {
    char res[17];
    long entero = num1 / num2;
    long residuo = num1 % num2;
    LCD_Clear();
    __delay_ms(2);
    LCD_Set_Cursor(0, 0);
    if (residuo == 0) {
        long_a_str(entero, res);
    } else {
        unsigned long decimales =
            ((unsigned long)residuo * 1000UL) / num2;
        sprintf(res, "%ld.%03lu", entero, decimales);
    }
    LCD_puts(res);
}

void main(void) {
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0xF0;
    PORTB = 0x0F;
    OPTION_REG &= 0b01111111;
    TRISC = 0;
    PORTC = 0;
    LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7};
    LCD_Init(lcd);
    LCD_Set_Cursor(0, 0);
    char buffer[17] = {0};
    uint8_t pos = 0;
    long num1 = 0;
    long num2 = 0;
    char operador = 0;
    char key;
    uint8_t esperando_num2 = 0;
    while (1) {
        key = Keypad_Scan();
        if (key == 0)
            continue;
        if (key == 'C') {
            num1 = 0;
            num2 = 0;
            operador = 0;
            esperando_num2 = 0;
            pos = 0;
            buffer[0] = '\0';
            LCD_Clear();
            __delay_ms(2);
            LCD_Set_Cursor(0, 0);
        }else if (key == '+' || key == '-' ||
                 key == '*' || key == '/') {
            if (pos > 0) {
                num1 = atol(buffer);
                operador = key;
                esperando_num2 = 1;
                pos = 0;
                buffer[0] = '\0';
                LCD_putc(key);
            }
        }else if (key == '=') {
            if (operador != 0 && pos > 0) {
                num2 = atol(buffer);
                if(operador == '/') {
                    if (num2 == 0) {
                        LCD_Clear();
                        __delay_ms(2);
                        LCD_Set_Cursor(0, 0);
                        LCD_puts("Error /0");
                        num1 = 0;
                        num2 = 0;
                        operador = 0;
                        esperando_num2 = 0;
                        pos = 0;
                        buffer[0] = '\0';
                        continue;
                    }
                    mostrar_division(num1, num2);
                    long entero = num1 / num2;
                    long residuo = num1 % num2;
                    if (residuo == 0) {
                        long_a_str(entero, buffer);
                    } else {
                        unsigned long decimales =
                            ((unsigned long)residuo * 1000UL) / num2;
                        sprintf(buffer,
                                "%ld.%03lu",
                                entero,
                                decimales);
                    }
                    pos = 0;
                    while (buffer[pos] != '\0')
                        pos++;
                    operador = 0;
                    esperando_num2 = 0;
                    continue;
                }else {
                    long resultado = 0;
                    if (operador == '+')
                        resultado = num1 + num2;
                    if (operador == '-')
                        resultado = num1 - num2;
                    if (operador == '*')
                        resultado = num1 * num2;
                    mostrar_resultado(resultado);
                    long_a_str(resultado, buffer);
                    pos = 0;
                    while (buffer[pos] != '\0')
                        pos++;
                    operador = 0;
                    esperando_num2 = 0;
                }
            }
        }else {
            if (pos < 16) {
                buffer[pos++] = key;
                buffer[pos] = '\0';
                LCD_putc(key);
            }
        }
    }
}