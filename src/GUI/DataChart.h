#ifndef _DATA_CHART_H_
#define _DATA_CHART_H_
#include <wx/wx.h>
#include "Controller.h"
#include <vector>
#include <math.h>

class DataChart : public wxPanel {
   private:
    Controller *controller;
    int maxTime = 1000; // ms

   public:
    DataChart(wxWindow *parent, wxSize size, Controller *controller);

    void paintEvent(wxPaintEvent &evt);
    void paintNow();

    void render(wxDC &dc);

    DECLARE_EVENT_TABLE()
};

#endif /* _DATA_CHART_H_ */