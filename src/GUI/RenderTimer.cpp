#include "RenderTimer.h"

RenderTimer::RenderTimer() : wxTimer() {
}

void RenderTimer::addSubscriber(wxPanel* panel){
    panels.push_back(panel);
}

void RenderTimer::Notify() {
    for(int i = 0; i < panels.size(); i++){
        panels.at(i)->Refresh();
    }
}

void RenderTimer::start() {
    wxTimer::Start(33);
}