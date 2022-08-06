#ifndef _PID_H_
#define _PID_H_
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int time;
    double position;
    double target;
    double error;
    double signal;
    double increment;
} pidState;

typedef struct {
    double P;
    double I;
    double D;
} pidSettings;

double P(pidState* state, pidSettings* settings);
double I(pidState* state, pidSettings* settings);
double PI(pidState* state, pidSettings* settings);

#endif /* _PID_H_ */