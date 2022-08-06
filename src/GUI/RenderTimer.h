#ifndef _RENDER_TIMER_H_
#define _RENDER_TIMER_H_

#include <wx/wx.h>
#include "LeverAnimation.h"
#include <vector>

class RenderTimer : public wxTimer {
    std::vector<wxPanel*> panels {};

   public:
    RenderTimer();
    void addSubscriber(wxPanel* panel);
    void Notify();
    void start();
};

#endif /* _RENDER_TIMER_H_ */