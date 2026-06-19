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

#define LED1 RD0
#define LED2 RD1
#define PWM_PERIOD 20

volatile uint8_t  contador = 0, pwm1 = 0, pwm2 = 0;  // ? uint8_t es suficiente (0-20)

//-----------------------------------------------------------------------------
void ADC_Init(void) {
    ANSEL  = 0x03;
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

//-----------------------------------------------------------------------------
void Timer0_Init(void) {
    OPTION_REG = 0x01;
    TMR0 = 240;
    T0IE = 1;
    GIE  = 1;
}

//-----------------------------------------------------------------------------
void __interrupt() ISR(void) {
    if (T0IF) {
        LED1 = (contador < pwm1) ? 1 : 0;
        LED2 = (contador < pwm2) ? 1 : 0;
        contador++;
        if (contador >= PWM_PERIOD)
            contador = 0;
        TMR0 = 240;
        T0IF = 0;
    }
}

//-----------------------------------------------------------------------------
void main(void) {
    TRISD = 0x00;
    PORTD = 0x00;
    ADC_Init();
    Timer0_Init();
    uint16_t adc1, adc2;
    while (1) {
        adc1 = ADC_Read(0);
        adc2 = ADC_Read(1);
        GIE = 0;
        pwm1 = (uint8_t)((uint32_t)adc1 * 20UL / 1023UL);
        pwm2 = (uint8_t)((uint32_t)adc2 * 20UL / 1023UL);
        GIE = 1;
        // __delay_ms(10);
    }
}