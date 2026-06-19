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
#define PWM_PERIOD  20
#define MOTOR RC2         // ? Mismo pin que usaría CCP1

volatile uint8_t contador = 0, pwm_duty = 0;

//=============================================================================
// ADC
//=============================================================================
void ADC_Init(void) {
    ANSEL  = 0x01;
    ANSELH = 0x00;
    ADCON0 = 0x01;
    ADCON1 = 0x80;
}

uint16_t ADC_Read(uint8_t channel) {
    ADCON0 &= 0x83;
    ADCON0 |= (channel << 2);
    __delay_us(5);
    GO_nDONE = 1;
    while (GO_nDONE);
    return ((uint16_t)ADRESH << 8) | ADRESL;
}

//=============================================================================
// TIMER0
//=============================================================================
void Timer0_Init(void) {
    OPTION_REG = 0x01;          // Prescaler 1:4
    TMR0 = 246;                 // 256-10 = 246 ? tick cada ~5µs
    T0IE = 1;
    GIE  = 1;
}

//=============================================================================
// ISR
//=============================================================================
void __interrupt() ISR(void) {
    if (T0IF) {
        MOTOR = (contador < pwm_duty) ? 1 : 0;
        contador++;
        if (contador >= PWM_PERIOD)
            contador = 0;
        TMR0 = 246;
        T0IF = 0;
    }
}

//=============================================================================
// MAIN
//=============================================================================
void main(void) {
    TRISC = 0x00;
    PORTC = 0x00;
    ADC_Init();
    Timer0_Init();
    uint16_t adc_val;
    while (1) {
        adc_val = ADC_Read(0);
        GIE = 0;
        pwm_duty = (uint8_t)((uint32_t)adc_val * 20UL / 1023UL);
        GIE = 1;
        __delay_ms(10);
    }
}