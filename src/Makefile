MCU=atmega168
F_CPU=1000000UL
CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS=-std=c99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
TARGET=PIDLever
SOURCES := $(shell find $(SOURCEDIR) -name '*.c')

all:
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SOURCES}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex

flash:
	avrdude -p ${MCU} -c usbasp -U flash:w:${TARGET}.hex:i -F -P usb -B 4

debug:
	${CC} ${CFLAGS} -o ${TARGET}.bin ${TARGET}.c
	simavr -g -m ${MCU} ${TARGET}.bin
	avr-gdb ${TARGET}.bin