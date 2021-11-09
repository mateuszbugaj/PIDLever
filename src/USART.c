#include <USART.h>

uint8_t numberBuffer[NUMBER_BUFFER_SIZE];
uint8_t commandReading = 1;
uint8_t newCommandAvailable = 0;
uint8_t index = 0;

void initUSART(void){
    // Set USART baud rate
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    // Use double speed mode
    UCSR0A |= (1 << U2X0);

    // Enable USART transmitter and receiver
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);

    // Receive complete interrut enable
    UCSR0B |= (1 << RXCIE0);

    // Set the size of data in frame to 8 bits
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void transmitByte(unsigned char data){
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = data;
}

void sendEndOfLine(){
    // transmitByte('\n');
    transmitByte('\r');
}

void printString(const char chars[]) {
    uint8_t i = 0;
    while (chars[i]) {
        transmitByte(chars[i]);
        i++;
    }

    sendEndOfLine();
}

/*
    Create integer from char array
*/
uint16_t makeNumber(uint8_t *charArray){
    uint8_t i = NUMBER_BUFFER_SIZE;
    uint16_t multiplier = 1;
    uint16_t number = 0;
    uint8_t insignificant = 1;

    while(i--){
        uint8_t byte = charArray[i];
        if(byte == 0){
            if(!insignificant){
                multiplier *= 10;
            }
        } else {
            insignificant = 0;
            number += (byte - '0') * multiplier;

            multiplier *= 10;
        }
    }

    return number;
}

uint8_t powOfTwo(uint8_t exponent){
    if(exponent == 0){
        return 1;
    }

    uint8_t value = 1;
    while(exponent--){
        value *= 2;
    }

    return value;
}

void decimalToBinary(uint8_t data, char *str){
    uint8_t i = 8;
    while(i-- > 0){
        uint8_t pow = powOfTwo(i);
        if(data >= pow){
            str[7-i] = '1';
            data -= pow;
        } else {
            str[7-i] = '0';
        }
    }
}

void decimalToHex(uint8_t value, char *str){
    if (value == 0)
        return;

    char temp[3] = {0, 0, 0};
    uint8_t reminder = 0, i = 0;

    while(value != 0){
        reminder = value % 16;
        if(reminder < 10){
            temp[i] = '0' + reminder;
        } else {
            temp[i] = 'A' + reminder - 10;

        }

        value /= 16;
        i++;
    }

    if(i==2){
        str[0] = temp[1];
        str[1] = temp[0];
    } else {
        str[0] = '0';
        str[1] = temp[0];
    }

}

void printLogNum(const char message[], int16_t data, enum NUMBER_FORMAT type){
    char log[100] = { 0 };

    switch (type){
        case DECIMAL:
            snprintf(log, 100, "LOG: %d : %s", data, message);
            break;

        case BINARY: ;
            char binaryNnumber[9] = { 0 };
            decimalToBinary(data, binaryNnumber);

            snprintf(log, 100, "LOG: 0b%s : %s", binaryNnumber, message);
            break;

        case HEX: ;
            char hexNumbes[3] = { 0 };
            decimalToHex(data, hexNumbes);

            snprintf(log, 100, "LOG: 0x%s : %s", hexNumbes, message);
            break;
    }

    printString(log);
}

/*
    Interrupt routine activated by the incoming data
*/
ISR(USART_RX_vect){
    uint8_t byte = UDR0;

    if(byte == '\r'){
        sendEndOfLine();
        commandNumber = makeNumber(numberBuffer);

        char log[100] = { 0 };
        snprintf(log, 100, "c: %s %d", command, commandNumber);
        printString(log);

        newCommandAvailable = 1;

        for(uint8_t i = 0; i < NUMBER_BUFFER_SIZE; i++){
            numberBuffer[i] = 0;
        }

        commandReading = 1;
        index = 0;

    } else {
        transmitByte(byte);
        if(commandReading){
            if(index == COMMAND_MAX_SIZE || byte == ' '){
                command[index] = '\0';
                commandReading = 0;
                index = 0;
            } else {
                command[index] = byte;
                index++;
            }

        } else {
            if(index < NUMBER_BUFFER_SIZE){
                numberBuffer[index] = byte;
                index++;
            }
        }
    }
}

uint8_t isNewCommand(){
    if(newCommandAvailable){
        newCommandAvailable = 0;
        return 1;
    }

    return 0;
}