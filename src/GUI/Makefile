CC = g++
CFLAGS=
WXFLAGS=`../../dep/linux/linux_lib/wx-config --cxxflags --libs`
TARGET=../../out/PIDLeverGUI
SOURCES := $(shell find $(SOURCEDIR) -name '*.cpp') ../PID.c

all:
	${CC} ${SOURCES} ${CFLAGS} ${WXFLAGS} -o ${TARGET}
