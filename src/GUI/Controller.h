#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wx/wx.h>

#include "../PID.h"

#define SAMPLES_NUMBER 150

class Controller {
   private:
    pidState currentState = pidState{0, 0, 90, 0, 1};
    pidSettings settings = pidSettings{0.01, 0.005, 0};
    double animationStartingPosition = 0;

   private:
    int checkIfNumber(std::string* str);

   public:
    bool runningSimulation = false;
    bool runningDevice = false;
    int sampleIndex = 0;
    pidState samples[SAMPLES_NUMBER];

   public:
    Controller();
    ~Controller();
    pidState* getCurrentState();
    pidSettings* getSettings();
    void updatePIDSettings(wxCommandEvent e);
    void runSimulationButtonHandler();
    void runDeviceButtonHandler();
    void addDataPoint(int time, int value);
    void clearDataPoints();
    void updateCurrentState(int newTime, double newPosition);
    void readDevice();
};

#endif /* _CONTROLLER_H_ */