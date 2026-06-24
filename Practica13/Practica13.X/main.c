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

#define MOTOR RC2
#define IN1   RD0
#define IN2   RD1

volatile uint8_t contador = 0;
volatile uint8_t pwm_duty = 0;
volatile uint8_t lado     = 0;

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

void Timer0_Init(void) {
    TMR0 = 246;
    T0IE = 1;
    GIE  = 1;
}

void __interrupt() ISR(void) {

    if (T0IF) {
        MOTOR = (contador < pwm_duty) ? 1 : 0;
        contador++;
        if (contador >= PWM_PERIOD)
            contador = 0;
        TMR0 = 246;
        T0IF = 0;
    }

    if (INTF) {
        lado = !lado;
        if (lado) {
            IN1 = 0;
            IN2 = 1;
        } else {
            IN1 = 1;
            IN2 = 0;
        }
        INTF = 0;
    }
}

void main(void) {
    TRISD = 0x00;
    PORTD = 0x00;
    TRISC = 0x00;
    PORTC = 0x00;
    TRISB = 0x01;
    ADC_Init();
    IN1 = 1;
    IN2 = 0;
    OPTION_REG = 0b00000001;
    INTF = 0;
    INTE = 1;
    GIE  = 1;
    Timer0_Init();
    uint16_t adc_val;
    while (1) {
        adc_val = ADC_Read(0);
        GIE = 0;
        pwm_duty = (uint8_t)((uint32_t)adc_val * 20UL / 1023UL);
        GIE = 1;
    }
}