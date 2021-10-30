#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/power.h>

int main(void){
    clock_prescale_set(clock_div_1); // CPU Clock at 8MHz
    DDRB |= (1 << PB4);

    while(1){
        PORTB ^= (1 << PB4);

        _delay_ms(500);
    }

    return 0;
}