#include <avr/io.h>

#define PWM_OUTPUT PB1
#define H_BRIDGE_DIR_A PB6
#define H_BRIDGE_DIR_B PB7

enum DIRECTION {RIGHT, LEFT};

/*
    Initialize 8-bit PWM on PWM_OUTPUT pin
*/
void initFastPWM();

/*
    Initialize H-Bridge control pins
*/
void initHBridgeDriver();

void setDirection(enum DIRECTION dir);

void setPWMDutyCycle(uint8_t dutyCycle);
