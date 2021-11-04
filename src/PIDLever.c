#include <TWI.h>
#include <USART.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>
#include <ROT_SENSOR.h>
#include <MotorControl.h>

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
void initAdcTemp() {
    ADMUX = (1 << ADLAR) | (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint8_t readPot() {
    return ADCH;
}

void driveMotor(uint8_t potReading){
    if(potReading < 128){
        setDirection(RIGHT);
        setPWMDutyCycle(128 - potReading);
    } else {
        setDirection(LEFT);
        setPWMDutyCycle(potReading - 128);
    }
}

int main(void) {
    /* Set CPU clock at 1 MHz */
    clock_prescale_set(clock_div_8);

    initFastPWM();
    initAdcTemp();
    initHBridgeDriver();
    initUSART();
    TWI_init(LOG_DISABLED);

    printLogNum("State", getAS5600Status(), BINARY);
    while (1) {
        driveMotor(readPot());
        printLogNum("Angle", getAngle(), DECIMAL);
        // _delay_ms(100);
    }

    return 0;
}