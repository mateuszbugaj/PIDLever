#include "LeverAnimation.h"

LeverAnimationPlane::LeverAnimationPlane(wxWindow* parent, wxSize size, Controller* controller) : wxPanel(parent, wxID_ANY, wxDefaultPosition, size) {
    this->controller = controller;
}

void LeverAnimationPlane::paintEvent(wxPaintEvent& evt) {
    wxPaintDC dc(this);
    render(dc);
}

void LeverAnimationPlane::paintNow() {
    wxClientDC dc(this);
    render(dc);
}

void LeverAnimationPlane::updateState(pidState* state, pidSettings* settings) {
    state->signal = PI(state, settings);
    state->position -= state->signal;
    controller->updateCurrentState(state->time + 33, state->position);

    if (state->position >= 360) {
        state->position = 0;
    }

    if (state->position < 0) {
        state->position = 359;
    }

    if (state->target >= 360) {
        state->target = 0;
    }

    if (state->target < 0) {
        state->target = 359;
    }

    controller->addDataPoint(controller->getCurrentState()->time, controller->getCurrentState()->position);
}

void LeverAnimationPlane::render(wxDC& dc) {
    dc.Clear();

    if (controller->runningSimulation) {
        updateState(controller->getCurrentState(), controller->getSettings());
    }

    // Position and target display
    dc.SetPen(wxPen(wxColor(236, 236, 236), 2));

    std::ostringstream positionStr;
    positionStr << std::fixed;
    positionStr << std::setprecision(2);
    positionStr << controller->getCurrentState()->position;
    dc.DrawText("Positon: " + positionStr.str(), wxPoint(10, 10));

    std::ostringstream targetStr;
    targetStr << std::fixed;
    targetStr << std::setprecision(2);
    targetStr << controller->getCurrentState()->target;
    dc.DrawText("Target: " + targetStr.str(), wxPoint(GetSize().GetWidth() - 120, 10));

    // Circle
    dc.SetBrush(wxColor(30, 30, 30));
    dc.SetPen(wxPen(wxColor(236, 236, 236), 2));
    dc.DrawCircle(wxPoint(GetSize().GetX() / 2, 120), 50);

    // Target
    double targetPositionRadians = (controller->getCurrentState()->target + 90) * (3.14 / 180);
    double xdes = -cos(targetPositionRadians) * 100;
    double ydes = sin(targetPositionRadians) * 100;

    dc.SetPen(wxPen(wxColor(236, 30, 30), 2));
    dc.DrawLine(GetSize().GetX() / 2, 120, GetSize().GetX() / 2 + xdes, 120 - ydes);

    // Lever
    double positionRadians = (controller->getCurrentState()->position + 90) * (3.14 / 180);
    double x = -cos(positionRadians) * 70;
    double y = sin(positionRadians) * 70;

    dc.SetPen(wxPen(wxColor(236, 236, 236), 10));
    dc.DrawLine(GetSize().GetX() / 2, 120, GetSize().GetX() / 2 + x, 120 - y);

    // Arc
    // dc.SetBrush(wxTransparentColor);
    // dc.SetPen(wxPen(wxColor(236, 236, 236), 3));

    // if (controller->getCurrentState()->error <= 0) {
    //     dc.DrawEllipticArc(
    //         wxCoord(GetSize().GetWidth() / 2 - 100),
    //         wxCoord(20),
    //         wxCoord(200),
    //         wxCoord(200),
    //         90 - controller->getCurrentState()->position + controller->getCurrentState()->error,
    //         90 - controller->getCurrentState()->position
    //         );
    // } else {
    //     dc.DrawEllipticArc(
    //         wxCoord(GetSize().GetWidth() / 2 - 100),
    //         wxCoord(20),
    //         wxCoord(200),
    //         wxCoord(200),
    //         90 - controller->getCurrentState()->position,
    //         90 - controller->getCurrentState()->position + controller->getCurrentState()->error);
    // }
}