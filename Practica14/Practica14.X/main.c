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

#define SERVO       RD0
#define PWM_PERIOD  200          // 200 * 100µs = 20ms
#define PWM_MIN     0           // 1.0ms ? 0°
#define PWM_MAX     30           // 2.0ms ? 180°

volatile uint16_t contador = 0;   // debe ser uint16_t para contar hasta 200
volatile uint8_t  pwm      = PWM_MIN;

void ADC_Init(void) {
    ANSEL  = 0x01;               // RA0 como entrada analógica
    ANSELH = 0x00;               // RA4..RA7 digitales
    ADCON0 = 0x81;               // ¡Encendido! canal 0, Fosc/32
    ADCON1 = 0x80;               // justificación derecha
}

unsigned int ADC_Read(void) {
    __delay_us(20);
    GO_nDONE = 1;
    while (GO_nDONE);
    return ((ADRESH << 8) + ADRESL);
}

void Timer0_Init(void) {
    OPTION_REG = 0x02;           // Prescaler 1:8
    TMR0 = 231;                  // 100µs por interrupción
    T0IE = 1;
    GIE  = 1;
}

void __interrupt() ISR(void) {
    if (T0IF) {
        SERVO = (contador < pwm) ? 1 : 0;

        contador++;
        if (contador >= PWM_PERIOD)
            contador = 0;

        TMR0 = 231;
        T0IF = 0;
    }
}

void main(void) {
    TRISD = 0x00;                // Puerto D como salida
    PORTD = 0x00;
    TRISA = 0xFF;                // Puerto A como entrada (innecesario pero no daña)

    ADC_Init();
    Timer0_Init();

    uint16_t adc_result;

    while (1) {
        adc_result = ADC_Read();

        GIE = 0;
        // Mapear 0-1023 ? PWM_MIN (10) a PWM_MAX (20)
        pwm = (uint8_t)(((uint32_t)adc_result * (PWM_MAX - PWM_MIN)) / 1023) + PWM_MIN;
        GIE = 1;

        __delay_ms(10);          // Pequeño retardo para estabilidad
    }
}