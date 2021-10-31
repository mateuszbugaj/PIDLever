#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#define PULSE_MIN 1000
#define PULSE_MAX 2000

void initTimerServo(void){
    TCCR1A |= (1 << WGM11);
}

int main(void){
    clock_prescale_set(clock_div_1); // CPU Clock at 8MHz
    DDRB |= (1 << PB4);

    while(1){
        PORTB ^= (1 << PB4);

        _delay_ms(500);
    }

    return 0;
}