#include "DataChart.h"

DataChart::DataChart(wxWindow *parent, wxSize size, Controller *controller) : wxPanel(parent, wxID_ANY, wxDefaultPosition, size) {
    this->controller = controller;
}

void DataChart::paintEvent(wxPaintEvent &evt) {
    wxPaintDC dc(this);
    render(dc);
}

void DataChart::paintNow() {
    wxClientDC dc(this);
    render(dc);
}

void DataChart::render(wxDC &dc) {
    double yAxisFactor = 380;
    int xAxisTranslationFactor = 40;
    int xAxisRightMargin = 50;
    int degreeDisplayValueOne = controller->samples[0].target-90 < 0 ? controller->samples[0].target+270 : controller->samples[0].target-90;
    int degreeDisplayValueTwo = controller->samples[0].target+90 > 360 ? controller->samples[0].target-270 : controller->samples[0].target+90;

    dc.Clear();

    dc.SetPen(wxPen(wxColor(236, 236, 236), 2));
    dc.DrawLine(40, GetSize().GetHeight()/2, GetSize().GetWidth()-xAxisRightMargin, GetSize().GetHeight()/2);
    dc.DrawText(std::to_string((int)controller->samples[0].target), wxPoint(0, GetSize().GetHeight()/2));

    dc.SetPen(wxPen(wxColor(236, 236, 236), 1));
    dc.DrawLine(
        40, 
        GetSize().GetHeight()/2 - (GetSize().GetHeight()/4)*(GetSize().GetHeight()/yAxisFactor), 
        GetSize().GetWidth()-xAxisRightMargin, 
        GetSize().GetHeight()/2 - (GetSize().GetHeight()/4)*(GetSize().GetHeight()/yAxisFactor));

    dc.DrawText(
        std::to_string(degreeDisplayValueOne), 
        wxPoint(0, GetSize().GetHeight()/2 - (GetSize().GetHeight()/4)*(GetSize().GetHeight()/yAxisFactor)));

    dc.DrawLine(
        40, 
        GetSize().GetHeight()/2 + (GetSize().GetHeight()/4)*(GetSize().GetHeight()/yAxisFactor), 
        GetSize().GetWidth()-xAxisRightMargin, 
        GetSize().GetHeight()/2 + (GetSize().GetHeight()/4)*(GetSize().GetHeight()/yAxisFactor));

    dc.DrawText(
        std::to_string(degreeDisplayValueTwo), 
        wxPoint(0, GetSize().GetHeight()/2 + (GetSize().GetHeight()/4)*(GetSize().GetHeight()/yAxisFactor)));
    
    int prevX = xAxisTranslationFactor;
    int prevY = GetSize().GetHeight()/2 + (controller->samples[0].position - controller->samples[0].target) * (GetSize().GetHeight() / yAxisFactor);;
    if(controller->runningSimulation || controller->runningDevice){
        for (int i = 1; i < SAMPLES_NUMBER; i++) {
            
            if(controller->samples[i].position == 0) continue;

            // int x = xAxisTranslationFactor + (((controller->samples[i].time - controller->samples[0].time) * (GetSize().GetWidth()-xAxisRightMargin)) / maxTime);
            int x = xAxisTranslationFactor + (((controller->samples[i].time) * (GetSize().GetWidth()-xAxisRightMargin)) / maxTime);
            double difference = controller->samples[i].position - controller->samples[i].target;
            if(difference > 180){
                difference -= 360;
            }
            double y = GetSize().GetHeight()/2 + difference * (GetSize().GetHeight() / yAxisFactor);

            dc.SetPen(wxPen(wxColor(0, 255, 0), 2));
            // dc.DrawPoint(x, y);
            dc.DrawLine(prevX, prevY, x, y);

            prevX = x;
            prevY = y;
        }
    }

    dc.SetPen(wxPen(wxColor(236, 236, 236), 2));
    dc.DrawLine(xAxisTranslationFactor, 0, xAxisTranslationFactor, GetSize().GetHeight() - 5);
}