#include <xc.h>
#include <stdint.h>

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 8000000

// Pines de salida
#define LED_VIDA1   LATDbits.LATD0
#define LED_VIDA2   LATDbits.LATD1
#define LED_VIDA3   LATDbits.LATD2
#define BUZZER      LATDbits.LATD3
// Botón
#define BOTON_SALTO PORTBbits.RB0

// LCD
#define LCD_RS      LATEbits.LATE0
#define LCD_EN      LATEbits.LATE1
#define LCD_DATA    LATD  // D4-D7 en RD4-RD7

// Variables del juego
uint8_t dino_pos = 0;          // 0=suelo, 1=saltando
uint8_t dino_anim = 0;         // 0=normal, 1=sin pata izq, 2=sin pata der
unsigned int pun = 0;
uint8_t vidas = 3;
uint8_t cactus_pos = 15;
uint8_t cactus_tipo = 0;      // 0=grande, 1=chico
uint8_t frame_cont = 0;

// Caracteres personalizados (tus datos)
const uint8_t character1[] = {7,23,22,31,14,10,15,0};
const uint8_t character2[] = {7,23,22,31,14,2,3,0};
const uint8_t character3[] = {7,23,22,31,14,8,12,0};
const uint8_t character4[] = {6,22,22,30,30,6,6,31};
const uint8_t character5[] = {0,0,12,13,15,12,12,31};
const uint8_t character6[] = {29,28,25,2,20,0,0,0};
const uint8_t character7[] = {28,29,24,2,8,0,0,0};

// ===================== FUNCIONES LCD 4 BITS =====================
void Lcd_Nibble(uint8_t nibble) {
    LCD_DATA = (LCD_DATA & 0x0F) | (nibble & 0xF0); // RD4-RD7
    LCD_EN = 1;
    __delay_us(1);
    LCD_EN = 0;
    __delay_us(100);
}

void Lcd_Cmd(uint8_t cmd) {
    LCD_RS = 0;
    Lcd_Nibble(cmd & 0xF0);
    Lcd_Nibble(cmd << 4);
    if (cmd == 0x01 || cmd == 0x02) __delay_ms(2);
}

void Lcd_Chr_CP(uint8_t data) {
    LCD_RS = 1;
    Lcd_Nibble(data & 0xF0);
    Lcd_Nibble(data << 4);
}

void Lcd_Init(void) {
    LCD_EN = 0;
    LCD_RS = 0;
    __delay_ms(30);
    Lcd_Nibble(0x30);
    __delay_ms(5);
    Lcd_Nibble(0x30);
    __delay_us(200);
    Lcd_Nibble(0x30);
    Lcd_Nibble(0x20);      // Modo 4 bits
    Lcd_Cmd(0x28);         // 2 líneas, 5x7
    Lcd_Cmd(0x0C);         // Display ON, cursor OFF
    Lcd_Cmd(0x06);         // Incremento automático
    Lcd_Cmd(0x01);         // Limpiar pantalla
    __delay_ms(2);
}

void Lcd_Set_Cursor(uint8_t fila, uint8_t col) {
    uint8_t addr = (fila == 1) ? 0x80 + col - 1 : 0xC0 + col - 1;
    Lcd_Cmd(addr);
}

void Lcd_Char(uint8_t fila, uint8_t col, uint8_t data) {
    Lcd_Set_Cursor(fila, col);
    Lcd_Chr_CP(data);
}

void Lcd_Out(uint8_t fila, uint8_t col, const char *str) {
    Lcd_Set_Cursor(fila, col);
    while (*str) Lcd_Chr_CP(*str++);
}

void Lcd_CustomChar(uint8_t cgram_addr, const uint8_t *pattern) {
    Lcd_Cmd(0x40 + (cgram_addr * 8)); // CGRAM address
    for (uint8_t i = 0; i < 8; i++) {
        Lcd_Chr_CP(pattern[i]);
    }
}

// ===================== SONIDO POR SOFTWARE =====================
void Tone(uint16_t freq, uint16_t duration_ms) {
    uint32_t period_us = 1000000UL / freq;
    uint32_t half = period_us / 2;
    uint32_t cycles = (uint32_t)duration_ms * 1000 / period_us;
    for (uint32_t i = 0; i < cycles; i++) {
        BUZZER = 1;
        __delay_us(half);
        BUZZER = 0;
        __delay_us(half);
    }
}

void sonido_pun(void) { Tone(800, 80); }
void sonido_fin(void) { Tone(500, 1500); }
void sonido_stop(void) { BUZZER = 0; }

// ===================== ACTUALIZAR LEDS =====================
void Actualizar_Leds(void) {
    LED_VIDA1 = (vidas >= 1);
    LED_VIDA2 = (vidas >= 2);
    LED_VIDA3 = (vidas >= 3);
}

// ===================== JUEGO =====================
void CustomChar1(uint8_t fila, uint8_t col) {
    Lcd_CustomChar(0, character1);
    Lcd_Char(fila, col, 0);
}
void CustomChar2(uint8_t fila, uint8_t col) {
    Lcd_CustomChar(0, character2);
    Lcd_Char(fila, col, 0);
}
void CustomChar3(uint8_t fila, uint8_t col) {
    Lcd_CustomChar(0, character3);
    Lcd_Char(fila, col, 0);
}
void CustomChar4(uint8_t fila, uint8_t col) {
    Lcd_CustomChar(1, character4);
    Lcd_Char(fila, col, 1);
}
void CustomChar5(uint8_t fila, uint8_t col) {
    Lcd_CustomChar(2, character5);
    Lcd_Char(fila, col, 2);
}
void CustomChar6(uint8_t fila, uint8_t col) {
    Lcd_CustomChar(3, character6);
    Lcd_Char(fila, col, 3);
}
void CustomChar7(uint8_t fila, uint8_t col) {
    Lcd_CustomChar(3, character7);
    Lcd_Char(fila, col, 3);
}

void vel_juego(void) {
    if (pun <= 10) __delay_ms(200);
    else if (pun <= 20) __delay_ms(150);
    else if (pun <= 30) __delay_ms(80);
    else if (pun <= 50) __delay_ms(70);
    else if (pun <= 100) __delay_ms(60);
    else __delay_ms(50);
}

void Generar_Cactus(void) {
    if (cactus_pos >= 15) {
        cactus_tipo = rand() % 2;   // 0 grande, 1 chico
        cactus_pos = 15;
    }
}

void Movimiento_Obstaculos(void) {
    if (cactus_pos > 0) cactus_pos--;
    else {
        pun++;
        sonido_pun();
        Generar_Cactus();
    }
}

void Dibujar_Pantalla(void) {
    Lcd_Cmd(0x01); // clear
    // Suelo
    Lcd_Out(2, 1, "----------------");

    // Dinosaurio (columna 1)
    if (dino_pos == 0) {
        if (dino_anim == 0) CustomChar1(2,1);
        else if (dino_anim == 1) CustomChar2(2,1);
        else CustomChar3(2,1);
    } else {
        CustomChar1(1,1); // saltando
    }

    // Cactus
    if (cactus_pos < 16) {
        if (cactus_tipo == 0)
            CustomChar4(2, cactus_pos+1);
        else
            CustomChar5(2, cactus_pos+1);
    }

    // Sol
    if (frame_cont % 2 == 0) CustomChar6(1,16);
    else CustomChar7(1,16);
}

void Checar_Colision(void) {
    if (dino_pos == 0 && cactus_pos >= 1 && cactus_pos <= 2) {
        vidas--;
        Actualizar_Leds();
        if (vidas > 0) {
            sonido_pun();
            __delay_ms(200);
            cactus_pos = 15;
        } else {
            sonido_fin();
            __delay_ms(500);
            vidas = 3;
            pun = 0;
            cactus_pos = 15;
            Actualizar_Leds();
        }
    }
}

// ===================== MAIN =====================
void main(void) {
    // Configurar puertos
    ANSEL = 0x00;
    ANSELH = 0x00;
    TRISA = 0x00;
    TRISB = 0xFF;          // RB0 entrada
    TRISC = 0x00;
    TRISD = 0x00;          // RD todo salida
    TRISE = 0x00;          // RE salidas
    // Pull-up en RB0
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0 = 1;
    // Apagar salidas
    PORTA = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    LED_VIDA1 = 0;
    LED_VIDA2 = 0;
    LED_VIDA3 = 0;
    BUZZER = 0;

    Lcd_Init();
    Actualizar_Leds();

    while (1) {
        // Botón de salto (activo bajo)
        if (BOTON_SALTO == 0) {
            if (dino_pos == 0) {
                dino_pos = 1;
                __delay_ms(250);
                dino_pos = 0;
            }
        }

        // Animación
        frame_cont++;
        if (frame_cont % 4 == 0) dino_anim = (dino_anim+1) % 3;

        // Movimiento y dibujo
        Movimiento_Obstaculos();
        Dibujar_Pantalla();
        Checar_Colision();
        Actualizar_Leds();
        vel_juego();
    }
}