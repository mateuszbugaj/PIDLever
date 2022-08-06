#include "Controller.h"

Controller::Controller() {
    for(int i = 0; i < SAMPLES_NUMBER; i++){
        samples[i].time = 0;
        samples[i].position = 0;
    }

    clearDataPoints();
}

Controller::~Controller() {
}

pidState* Controller::getCurrentState() {
    return &currentState;
}

pidSettings* Controller::getSettings() {
    return &settings;
}

void Controller::updatePIDSettings(wxCommandEvent e) {
    wxTextCtrl* textInputElement = (wxTextCtrl*)e.GetEventObject();

    if (textInputElement->GetValue().ToStdString() == "") {
        return;
    }

    double value = std::stod(textInputElement->GetValue().ToStdString(), nullptr);

    if (textInputElement->GetName().GetData().AsString() == "P") {
        settings.P = value;
        return;
    }

    if (textInputElement->GetName().GetData().AsString() == "I") {
        settings.I = value;
        return;
    }

    if (textInputElement->GetName().GetData().AsString() == "D") {
        settings.D = value;
        return;
    }
}

void Controller::runSimulationButtonHandler() {
    currentState.increment = 0;

    if (runningSimulation) {
        runningSimulation = false;
        clearDataPoints();
        return;
    }

    if (runningDevice) {
        runningDevice = false;
    }

    runningSimulation = true;
    clearDataPoints();
}

void Controller::runDeviceButtonHandler() {
    currentState.increment = 0;

    if (runningDevice) {
        runningDevice = false;
        clearDataPoints();

        return;
    }

    if (runningSimulation) {
        runningSimulation = false;
    }

    runningDevice = true;
    std::cout << "rec\r";
}

void Controller::addDataPoint(int time, int value){
    pidState newSample;
    newSample.time = time;
    newSample.position = value;
    newSample.target = currentState.target;

    newSample.error = -360 * (newSample.target - newSample.position > 180 ? -1 : 0) + newSample.target - newSample.position;
    if (newSample.position - newSample.target > 180) {
        newSample.error = (newSample.position - newSample.target) - 360;
    } else if (newSample.position - newSample.target < -180) {
        newSample.error = (newSample.position - newSample.target) + 360;
    } else {
        newSample.error = newSample.position - newSample.target;
    }

    for(int i = 0; i < SAMPLES_NUMBER; i++){
        if(samples[i].time == newSample.time){
            sampleIndex = i;
        }
    }

    samples[sampleIndex].time = newSample.time;
    samples[sampleIndex].position = newSample.position;
    samples[sampleIndex].error = newSample.error;
    samples[sampleIndex].target = currentState.target;

    if(!runningSimulation){
        // currentState.position = newSample.position;
        updateCurrentState(newSample.time, newSample.position);
    }

    if (sampleIndex == SAMPLES_NUMBER - 1) {
        for (int i = 0; i < SAMPLES_NUMBER - 1; i++) {
            samples[i] = samples[i + 1];
        }
    } else {
        sampleIndex++;
    }
}

void Controller::clearDataPoints() {
    sampleIndex = 0;
    for (int i = 0; i < SAMPLES_NUMBER; i++) {
        samples[i].time = 0;
        samples[i].position = 0;
        samples[i].target = 0;
        samples[i].error = 0;
    }
}

void Controller::readDevice() {
    std::string inputLine;
    std::getline(std::cin, inputLine);

    if(inputLine.size() == 0) return;
    inputLine = inputLine.erase(0, 1);

    if (!isdigit(inputLine.at(0))) return;
    
    std::string timeSubstring = inputLine.substr(0, inputLine.find(" "));
    std::string positionSubstring = inputLine.substr(inputLine.find(" ") + 1, inputLine.size() - (inputLine.find(" ") + 1));

    // addDataPoint(std::stoi(timeSubstring), std::stoi(positionSubstring)* 0.087890625);

    pidState newSample;
    newSample.time = std::stoi(timeSubstring);
    newSample.position = std::stoi(positionSubstring)* 0.087890625;
    newSample.target = currentState.target;


    if(!runningSimulation){
        // currentState.position = newSample.position;
        updateCurrentState(0, newSample.position);
    }

    if(newSample.time == 60000){
        return;
    }

    newSample.error = -360 * (newSample.target - newSample.position > 180 ? -1 : 0) + newSample.target - newSample.position;
    if (newSample.position - newSample.target > 180) {
        newSample.error = (newSample.position - newSample.target) - 360;
    } else if (newSample.position - newSample.target < -180) {
        newSample.error = (newSample.position - newSample.target) + 360;
    } else {
        newSample.error = newSample.position - newSample.target;
    }

    // for(int i = 0; i < SAMPLES_NUMBER; i++){
    //     if(samples[i].time == newSample.time){
    //         sampleIndex = i;
    //     }
    // }

    samples[sampleIndex].time = newSample.time;
    samples[sampleIndex].position = newSample.position;
    samples[sampleIndex].error = newSample.error;
    samples[sampleIndex].target = currentState.target;

    // if (sampleIndex == SAMPLES_NUMBER - 1) {
    //     for (int i = 0; i < SAMPLES_NUMBER - 1; i++) {
    //         samples[i] = samples[i + 1];
    //     }
    // } else {
    //     sampleIndex++;
    // }

    // std::cout << newSample.time << '\n';
    // std::cout << sampleIndex << "> " << samples[sampleIndex].time << " : " << samples[sampleIndex].position << '\n';
    sampleIndex++;
}

void Controller::updateCurrentState(int newTime, double newPosition) {
    currentState.time = newTime;
    currentState.position = newPosition;
    currentState.error = -360 * (currentState.target - currentState.position > 180 ? -1 : 0) + currentState.target - currentState.position;
    // currentState.difference = currentState.position - currenState.oldPosition;
}