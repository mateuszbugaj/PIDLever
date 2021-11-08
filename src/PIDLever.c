#include <TWI.h>
#include <USART.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>
#include <ROT_SENSOR.h>
#include <MotorControl.h>

/*
    Initialize ADC to read analog potentiometer
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
void initAdcTemp() {
    ADMUX = (1 << ADLAR) | (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

/* Read target rotation from analog potentiometer */
uint8_t readPot() {
    return ADCH;
}

/* Set direction and duty cycle of pwm for DC motor */
void driveMotor(int16_t signal){
    if(signal < 0){
        setDirection(RIGHT);
        signal *= -1;
    } else {
        setDirection(LEFT);
    }


    setPWMDutyCycle(signal);
}

/*
    Translate 8-bit target rotation reading 
    into 12-bit number
*/
uint16_t readTargetRotation(uint8_t reading){
    return reading * 16;
}

int16_t PID(uint16_t target, uint16_t reading){
    int16_t error = target - reading;
    int16_t signal = 0;
    float gain = 1.3;
    signal = (error/16) * gain;
    printLogNum("Signal", signal, DECIMAL);

    return signal;
}

int main(void) {
    /* Set CPU clock at 1 MHz */
    clock_prescale_set(clock_div_8);

    initFastPWM();
    initAdcTemp();
    initHBridgeDriver();
    initUSART();
    TWI_init(LOG_DISABLED);

    /* Represends 0-360 degrees as 12-bit number stored in 16-bit register */
    uint16_t targetAngle, currentAngle;

    printLogNum("State", getAS5600Status(), BINARY);
    while (1) {
        targetAngle = readTargetRotation(readPot());
        currentAngle = getAngle();

        driveMotor(PID(targetAngle, currentAngle));
    }

    return 0;
}