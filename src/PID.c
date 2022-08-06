#include "PID.h"

double PI(pidState* state, pidSettings* settings){
    state->error = -360 * (state->target - state->position > 180 ? -1 : 0) + state->target - state->position;
    if (state->position - state->target > 180) {
        state->error = (state->position - state->target) - 360;
    } else if (state->position - state->target < -180) {
        state->error = (state->position - state->target) + 360;
    } else {
        state->error = state->position - state->target;
    }

    state->increment = state->increment + state->error;

    return state->error * settings->P + state->increment * settings->I;
}