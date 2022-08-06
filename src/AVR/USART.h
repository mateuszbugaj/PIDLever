#ifndef _USART_H_
#define _USART_H_

#ifndef BAUD
#define BAUD 9600
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <stdbool.h>

#define USART_BUFFER_SIZE 20

char usartLastCommand[USART_BUFFER_SIZE];

void usartInit(void);
void usartPrint(char content[]);
void usartPrintNum(uint16_t number);
bool usartCompareStr(char* s1, char* s2);
uint16_t* usartGetNumbers(char str[], uint16_t* buffer);
bool usartContatinsStr(char* s1, char* s2);
bool usartCommandProcessed(); // This can be a command FIFO queue


#endif /* _USART_H_ */