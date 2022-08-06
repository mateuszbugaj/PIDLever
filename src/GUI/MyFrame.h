#ifndef _MY_FRAME_H_
#define _MY_FRAME_H_

#include <wx/wx.h>

#include "Controller.h"
#include "DataChart.h"
#include "LeverAnimation.h"
#include "RenderTimer.h"

class MyFrame : public wxFrame {
   public:
    RenderTimer *timer;
    Controller *controller;
    LeverAnimationPlane *leverAnimation;
    DataChart *dataChart;

   public:
    MyFrame(const wxString &title, Controller *controller);
    ~MyFrame();
    void onClose(wxCloseEvent &evt) {
        evt.Skip();
    }

   private:
    wxBoxSizer* createValueInput(std::string name);
};

#endif