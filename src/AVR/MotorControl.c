#include <MotorControl.h>

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
void initFastPWM(void) {
    TCCR1A = (1 << COM1A1) | (1 << WGM10);
    TCCR1B = (1 << CS11) | (1 << WGM12);

    DDRB |= (1 << PWM_OUTPUT);
}

void initHBridgeDriver() {
    DDRB |= (1 << H_BRIDGE_DIR_A) | (1 << H_BRIDGE_DIR_B);

    PORTB &= ~(1 << H_BRIDGE_DIR_B);
    PORTB |= (1 << H_BRIDGE_DIR_A);
}

void setPWMDutyCycle(uint8_t dutyCycle) {
    OCR1A = dutyCycle;
}

void setDirection(enum DIRECTION dir) {
    if (dir == RIGHT) {
        PORTB &= ~(1 << H_BRIDGE_DIR_B);
        PORTB |= (1 << H_BRIDGE_DIR_A);
    } else {
        PORTB &= ~(1 << H_BRIDGE_DIR_A);
        PORTB |= (1 << H_BRIDGE_DIR_B);
    }
}