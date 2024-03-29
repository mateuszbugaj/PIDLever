#include <avr/io.h>
#include <USART.h>
#include <TWI.h>
#include <stdio.h>

bool logActive;
uint8_t lastStatusCode = 0;

inline void decodeStatus(uint8_t code);

void TWI_init(bool log){
    PORTC |= (1 << PC4) | (1 << PC5); // pull-up resistors for SCL and SDA

    TWBR = 32; // SCL 100kHz for 8MHz / 42 SCL 10 kHz for 1MHz
    TWCR |= (1 << TWEN); // Enable TWI

    logActive = log;
}

void waitForComplete(void){
    if(lastStatusCode == 0x48 || lastStatusCode == 0x20){
        if(logActive){
            // USART_print("Error\n\r", 1);
            usartPrint("Error\n\r");
        }
        return;
    }
    while(!(TWCR & (1<<TWINT)));
}

void TWI_start(void){
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    decodeStatus((TWSR & 0xF8));
    waitForComplete();
}

uint8_t generateAddress(uint8_t address, uint8_t direction){
    return address * 2 + direction;
}

void TWI_sendAddress(unsigned char address, unsigned char direction){
    uint8_t SLA_X = generateAddress(address, direction);
    TWDR = SLA_X;
    TWCR = (1<<TWINT) | (1<<TWEN);
    decodeStatus((TWSR & 0xF8));
    waitForComplete();
}


void TWI_sendData(unsigned char data){
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);
    decodeStatus((TWSR & 0xF8));
    waitForComplete();
}

unsigned char TWI_getData(bool ack){
    if(ack){
        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
    }
     else{
        TWCR = (1<<TWINT) | (1<<TWEN);
     }

    decodeStatus((TWSR & 0xF8));
    waitForComplete();
    return (TWDR);
}

void TWI_stop(void){
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

void decodeStatus(uint8_t code){
    lastStatusCode = code;
    if(!logActive)
        return;

    // char message[70] = { 0 };
    // switch (code){
    //     case 0x08:
    //         sprintf(message, "A START condition has been transmited\n\r");
    //         break;

    //     case 0x10:
    //         sprintf(message, "A START condition has been transmited\n\r");
    //         break;

    //     case 0x18:
    //         sprintf(message, "SLA+W has been transmitted, ACK has been received\n\r");
    //         break;

    //     case 0xF8:
    //         sprintf(message, "ERROR: No relevant state information available; TWINT = “0”\n\r");
    //         break;

    //     case 0x48:
    //         sprintf(message, "ERROR: SLA+R has been transmitted; NOT ACK has been received\n\r");
    //         break;

    //     case 0x20:
    //         sprintf(message, "ERROR: SLA+W has been transmitted; NOT ACK has been received\n\r");
    //         break;

    //     case 0x40:
    //         sprintf(message, "SLA+R has been transmitted; ACK has been received\n\r");
    //         break;

    //     case 0x28:
    //         sprintf(message, "Data byte has been transmitted; ACK has been received\n\r");
    //         break;

    //     case 0x50:
    //         sprintf(message, "Data byte has been received; ACK has been returned\n\r");
    //         break;

    //     case 0x58:
    //         sprintf(message, "Data byte has been received; NOT ACK has been returned\n\r");
    //         break;
        
    //     default:
    //         sprintf(message, "Unrecognized code has been received\n\r");
    //         break;
    // }

    // // printLogNum(message, code, HEX);
    // USART_print(message, 1);
}