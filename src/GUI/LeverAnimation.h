#ifndef _LEVER_ANIMATION_PLANE_CPP_
#define _LEVER_ANIMATION_PLANE_CPP_
#include <math.h>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <wx/wx.h>

#include "Controller.h"
#include "../PID.h"

class LeverAnimationPlane : public wxPanel {
    public:
    Controller* controller;

   public:
    LeverAnimationPlane(wxWindow *parent, wxSize size, Controller* controller);

    void paintEvent(wxPaintEvent &evt);
    void paintNow();

    void render(wxDC &dc);
    void updateState(pidState* state, pidSettings* settings);

    void fun(wxTimerEvent& event);

    DECLARE_EVENT_TABLE()
};

#endif /* _LEVER_ANIMATION_PLANE_CPP_ */