#include <wx/wx.h>

#include <iostream>
#include <thread>

#include "DataChart.h"
#include "LeverAnimation.h"
#include "MyFrame.h"

class MyApp : public wxApp {
   public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

class InputThread: public wxThread {
    private:
        Controller* controller;

    public:
    InputThread(Controller* controller) : wxThread(wxTHREAD_DETACHED) {
        this->controller = controller;
        if (wxTHREAD_NO_ERROR == Create()) {
            Run();
        }
    }

   protected:
    virtual ExitCode Entry() {
        while (!TestDestroy()) {
            controller->readDevice();
        }
        return static_cast<ExitCode>(NULL);
    }
};

bool MyApp::OnInit() {
    Controller* controller = new Controller();
    InputThread* inputThread = new InputThread(controller);
    MyFrame* frame = new MyFrame("PID Lever", controller);
    frame->Show();

    return true;
}

BEGIN_EVENT_TABLE(LeverAnimationPlane, wxPanel)
EVT_PAINT(LeverAnimationPlane::paintEvent)
EVT_CLOSE(MyFrame::onClose)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(DataChart, wxPanel)
EVT_PAINT(DataChart::paintEvent)
END_EVENT_TABLE()