#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <string.h>
#include <util/delay.h>

#include "../PID.h"
#include "MotorControl.h"
#include "ROT_SENSOR.h"
#include "TWI.h"
#include "USART.h"

#define ACTION_LED PB0
#define SAMPLES_NUMBER 150

uint8_t recordingSamplingInterval = 30;  // ms
uint8_t idleSamplingInterval = 30;
uint8_t currentSamplingInterval = 50;

typedef struct {
    uint16_t time;
    uint16_t value;
} Sample;

Sample samples[SAMPLES_NUMBER];
uint16_t sampleIndex = 0;
uint8_t recording = 0;
uint8_t sending = 0;
uint8_t idle = 1;
uint16_t prevTime = 0; // used for idle position transmission

pidState currentState;
pidSettings settings;

char message[20] = {0};

bool runMotor = false;
uint16_t currentAngle;  // Represends 0-360 degrees as 12-bit number stored in 16-bit register
uint8_t dutyCycle = 0;

/* 8-bit Timer/Counter0 */
void setupTimer() {
    TCCR0A = (1 << WGM01);               // CTC Mode, OCRA - TOP
    TCCR0B = (1 << CS02) | (1 << CS00);  // Prescaler to 1024
    OCR0A = idleSamplingInterval;            // '10' with prescaler 1024 and 1Mhz CPU it gives around 10ms
    TIMSK0 = (1 << OCIE0A);              // Output compare match A interrupt enable
    TIFR0 = 0;                           // clear flags

    sei();
}

double _abs(double input) {
    return input >= 0 ? input : -1 * input;
}

void printSamples(uint8_t number){
    idle = 0;
    for (uint8_t i = 0; i < ((number > SAMPLES_NUMBER || number == 0) ? SAMPLES_NUMBER : number); i++) {
        usartPrintNum(samples[i].time);
        usartPrint(" ");
        usartPrintNum(samples[i].value);
        usartPrint("\n\r");
    }
    idle = 1;
}

int main(void) {
    /* Set CPU clock at 1 MHz */
    clock_prescale_set(clock_div_8);
    setupTimer();
    initFastPWM();
    initHBridgeDriver();
    usartInit();
    TWI_init(LOG_DISABLED);
    DDRB |= (1 << ACTION_LED);

    currentState.position = 0;
    currentState.target = 90;

    settings.P = 0.1;
    settings.I = 0.00;
    settings.D = 0;

    // Debug: Show rotation sensor's status
    // USART_print("State: ", 1, getAS5600Status());
    // USART_print("\n\r", 0);

    // USART_print("Start\n\r", 0);
    usartPrint("Start\n\r");
    while (1) {
        if(!usartCommandProcessed()){
            if(usartContatinsStr(usartLastCommand, "led on")){
                PORTB |= (1 << ACTION_LED);
            }

            if(usartContatinsStr(usartLastCommand, "led off")){
                PORTB &= ~(1 << ACTION_LED);
            }

            if(usartContatinsStr(usartLastCommand, "led switch")){
                usartPrint("Switch\n\r");
                uint16_t tempBuffer[5] = {0};
                usartPrint("Number: ");
                usartPrintNum(usartGetNumbers(usartLastCommand, tempBuffer)[0]);
                usartPrint("\n\r");
                for(uint8_t i = 0; i < usartGetNumbers(usartLastCommand, tempBuffer)[0]; i++){
                    PORTB |= (1 << ACTION_LED);
                    _delay_ms(1000);
                    PORTB &= ~(1 << ACTION_LED);
                    _delay_ms(1000);
                }
            }

            if(usartContatinsStr(usartLastCommand, "idle off")){
                idle = 0;
            }

            if(usartContatinsStr(usartLastCommand, "idle on")){
                idle = 1;
            }

            if(usartContatinsStr(usartLastCommand, "rec")){
                for (uint16_t i = 0; i < SAMPLES_NUMBER; i++) {
                    samples[i].time = 0;
                    samples[i].value = 0;
                }
                
                recording = 1;
                runMotor = true;
                idle = 0;
                sampleIndex = 0;
                currentSamplingInterval = recordingSamplingInterval;
                OCR0A = currentSamplingInterval;
                PORTB |= (1 << ACTION_LED);
            }

            if(usartContatinsStr(usartLastCommand, "new target")){
                uint16_t tempBuffer[1] = {0};
                currentState.target = usartGetNumbers(usartLastCommand, tempBuffer)[0];
            }

            if(usartContatinsStr(usartLastCommand, "get target")){
                usartPrintNum(currentState.target);
            }

            if(usartContatinsStr(usartLastCommand, "motor on")){
                runMotor = true;
            }

            if(usartContatinsStr(usartLastCommand, "motor off")){
                runMotor = false;
            }

            if(usartContatinsStr(usartLastCommand, "set pid")){
                uint16_t tempBuffer[1] = {0};
                settings.P = usartGetNumbers(usartLastCommand, tempBuffer)[0]/1000.0;
                settings.I = usartGetNumbers(usartLastCommand, tempBuffer)[1]/1000.0;
                settings.D = usartGetNumbers(usartLastCommand, tempBuffer)[2]/1000.0;
            }


            if(usartContatinsStr(usartLastCommand, "get pid")){
                usartPrint("P: ");
                usartPrintNum((uint16_t)(settings.P * 1000));
                usartPrint("/1000");
                usartPrint(" I: ");
                usartPrintNum((uint16_t)(settings.I * 1000));
                usartPrint("/1000");
                usartPrint(" D: ");
                usartPrintNum((uint16_t)(settings.D * 1000));
                usartPrint("/1000");
                usartPrint("\n\r");
            }
        }

        currentAngle = getAngle();

        currentState.position = currentAngle * 0.087890625;
        currentState.signal = PI(&currentState, &settings);

        if (currentState.signal > 0) {
            setDirection(RIGHT);
            currentState.signal *= -1;
        } else {
            setDirection(LEFT);
        }

        if (_abs(currentState.signal) >= 20) {
            dutyCycle = 255;
        } else {
            dutyCycle = (uint8_t)(_abs(currentState.signal) * 12.75);
        }

        if(runMotor){
            setPWMDutyCycle(dutyCycle);
        } else {
            setPWMDutyCycle(0);
        }

        // USART_print("", 1, currentAngle);
        // USART_print("\n\r", 0);
    }

    return 0;
}

ISR(TIMER0_COMPA_vect) {
    if(idle){
        OCR0A = idleSamplingInterval;
        usartPrintNum(60000);
        usartPrint(" ");
        usartPrintNum(currentAngle);
        usartPrint("\n\r");
    }

    if (sampleIndex >= SAMPLES_NUMBER) {
        currentSamplingInterval = idleSamplingInterval;
        OCR0A = currentSamplingInterval;
        sampleIndex = 0;
        if(recording){
            recording = 0;
            prevTime = 0;
            runMotor = false;
            PORTB &= ~(1 << ACTION_LED);
            // addCommand("sen", SAMPLES_NUMBER);
            printSamples(SAMPLES_NUMBER);
        }
        return;
    }

    if (sampleIndex == SAMPLES_NUMBER / 2) {
        // currentSamplingInterval = recordingSamplingInterval * 2;
        // OCR0A = currentSamplingInterval;
    }

    samples[sampleIndex].time = sampleIndex == 0 ? 0 : samples[sampleIndex - 1].time + currentSamplingInterval;

    samples[sampleIndex].value = currentAngle;
    sampleIndex++;
}
