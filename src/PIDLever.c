#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>

#define PWM_OUTPUT PB1
#define H_BRIDGE_DIR_A PB6
#define H_BRIDGE_DIR_B PB7

/*
    TCCR1A
        Clear OC1A on Compare Match, set OC1A at BOTTOM
        Set waveform generation mode to fast 8-bit PWM

    TCCR1B
        Set prescaler to /8 (no prescaling)
        This gives 1000 cycles per ms. for 1 MHz clock

    PWM Frequency
        CPU Clock / (Prescaler * [1 + TOP])
        Where TOP is equal to 0x00FF (255)

        1_000_000 / (8 * 256) = 488 Hz
*/
void initFastPWM(void){
    TCCR1A = (1 << COM1A1) | (1 << WGM10);
    TCCR1B = (1 << CS11) | (1 << WGM12);

    DDRB |= (1 << PWM_OUTPUT);
}

/*
    Initialize ADC
    ADMUX
        Set AREF as voltage reference)
        Left adjust the results
        Set input channel for PC0 (MUX[3:0] = 0)
    ADCSRA
        Enable ADC
        Start the first conversion
        Enable auto trigger of ADC (Probably should set this to Timer/Counter with ADCSRB)
        Enable ADC interrupt
        Set clock prescaler to 128
*/
void initAdcTemp(){
    ADMUX = (1 << ADLAR) | (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint8_t readPot(){
    return ADCH;
}

void initHBridgeDriver(){
    DDRB |= (1 << H_BRIDGE_DIR_A) | (1 << H_BRIDGE_DIR_B);

    PORTB &= ~(1 << H_BRIDGE_DIR_B);
    PORTB |= (1 << H_BRIDGE_DIR_A);
}

int main(void) {
    /* Set CPU clock at 1 MHz */
    clock_prescale_set(clock_div_8);
    initFastPWM();
    initAdcTemp();
    initHBridgeDriver();

    while (1) {
        OCR1A = readPot();
    }

    return 0;
}