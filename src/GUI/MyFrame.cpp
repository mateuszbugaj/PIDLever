#include "MyFrame.h"

MyFrame::MyFrame(const wxString &title, Controller *controller) : wxFrame(nullptr, wxID_ANY, title, wxPoint(750, 200), wxDefaultSize) { // , wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER
    this->controller = controller;
    timer = new RenderTimer();
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    // Lever animation
    leverAnimation = new LeverAnimationPlane((wxFrame *)this, wxSize(400, 230), controller);
    leverAnimation->SetBackgroundColour(wxColor(44, 44, 44));
    leverAnimation->SetSize(wxSize(340, 340));
    sizer->Add(leverAnimation, 0, wxEXPAND);
    sizer->Add(0, 10); // space

    timer->addSubscriber(leverAnimation);

    // Values display
    wxBoxSizer *valuesSizer = new wxBoxSizer(wxHORIZONTAL);
    valuesSizer->Add(createValueInput("P"));
    valuesSizer->Add(createValueInput("I"));
    valuesSizer->Add(createValueInput("D"));

    sizer->Add(valuesSizer, 0, wxALIGN_CENTER_HORIZONTAL);
    sizer->Add(0, 10); // space

    // State and error chart
    dataChart = new DataChart((wxFrame *)this, wxSize(600, 360), controller);
    dataChart->SetBackgroundColour(wxColor(44, 44, 44));
    dataChart->SetSize(wxSize(340, 340));
    sizer->Add(dataChart, 0, wxEXPAND);
    sizer->Add(0, 10); // space

    timer->addSubscriber(dataChart);

    // Options buttons
    wxBoxSizer *optionsSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *runSimulationButton = new wxButton(this, wxID_ANY, "Run Simulation");
    wxButton *runDeviceButton = new wxButton(this, wxID_ANY, "Run Device");
    runSimulationButton->Bind(wxEVT_BUTTON, [controller, runSimulationButton, runDeviceButton](wxCommandEvent &e){ 
        controller->runSimulationButtonHandler();
        // runSimulationButton->SetForegroundColour(controller->guiSettings.runningSimulation? wxColor(247, 20, 20): wxColor(247, 247, 247));

        if(controller->runningSimulation){
            runSimulationButton->SetForegroundColour(wxColor(247, 20, 20));
            runDeviceButton->SetForegroundColour(wxColor(247, 247, 247));
        } else {
            runSimulationButton->SetForegroundColour(wxColor(247, 247, 247));
        }
    });
    runDeviceButton->Bind(wxEVT_BUTTON, [controller, runDeviceButton, runSimulationButton](wxCommandEvent &e){
        controller->runDeviceButtonHandler();

        if(controller->runningDevice){
            runDeviceButton->SetForegroundColour(wxColor(247, 20, 20));
            runSimulationButton->SetForegroundColour(wxColor(247, 247, 247));
        } else {
            runDeviceButton->SetForegroundColour(wxColor(247, 247, 247));
        }

    });
    optionsSizer->Add(runSimulationButton, 0, wxRIGHT, 15);
    optionsSizer->Add(runDeviceButton, 0, wxRIGHT, 15);

    sizer->Add(optionsSizer, 0, wxALIGN_LEFT);
    sizer->Add(0, 10); // space

    // Connected port name info
    wxBoxSizer *inputSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *lastLine = new wxStaticText(this, wxID_ANY, "...");
    // wxStaticText *lastLine = new wxStaticText(this, wxID_ANY, controller->device->lastLines[0]);
    inputSizer->Add(lastLine, 0, wxEXPAND | wxLEFT | wxTOP, 10);

    sizer->Add(inputSizer, 0, wxEXPAND);
    sizer->Add(0, 10); // space


    mainSizer->Add(sizer, 0, wxEXPAND | wxALL, 10);
    this->SetSizerAndFit(mainSizer);
    timer->start();
}

wxBoxSizer* MyFrame::createValueInput(std::string name){
    Controller* controller = this->controller;
    wxBoxSizer *valueSizer = new wxBoxSizer(wxHORIZONTAL);
    double initialValue = -1;
    if(name == "P"){
        initialValue = controller->getSettings()->P;
    } else if(name == "I"){
        initialValue = controller->getSettings()->I;
    } else if(name == "D"){
        initialValue = controller->getSettings()->D;
    }

    std::ostringstream initialValueStr;
    initialValueStr << std::fixed;
    initialValueStr << std::setprecision(5);
    initialValueStr << initialValue;

    wxTextCtrl *valueInput = new wxTextCtrl(this, wxID_ANY, initialValueStr.str(), wxDefaultPosition, wxDefaultSize);
    valueInput->SetName(name);
    valueInput->Bind(wxEVT_TEXT, [controller](wxCommandEvent e){ 
        controller->updatePIDSettings(e);
    });
    valueInput->SetMaxSize(wxSize(80, 40));
    valueSizer->Add(10, 0);
    valueSizer->Add(new wxStaticText(this, wxID_ANY, name + ": "), 1, wxEXPAND | wxLEFT | wxTOP, 7);
    valueSizer->Add(valueInput);
    valueSizer->Add(10, 0);
    return valueSizer;
}

MyFrame::~MyFrame() {
    timer->Stop();
    delete timer;
}