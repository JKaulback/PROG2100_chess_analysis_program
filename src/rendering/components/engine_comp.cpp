#include "engine_comp.h"

namespace CtrlCfg = Config::Controls;

EngineComp::EngineComp(const ChessAnalysisProgram& controller) :
    controller(controller) {
    
    isEngineRunning = false;
}

void EngineComp::draw() const {
    drawEngineStatus();
    drawEngineAnalysis();
    drawEngineControls();
}

void EngineComp::setEngineRunning(bool isRunning) {
    isEngineRunning = isRunning;
}

void EngineComp::drawEngineStatus() const {
    std::string text = 
        (isEngineRunning) ?
        "Engine enabled" :
        "Engine disabled";

    DrawText(text.c_str(),
        CtrlCfg::START_DRAW_X - 300,
        CtrlCfg::START_DRAW_Y - 200,
        CtrlCfg::FONT_SIZE_PX, CtrlCfg::FONT_COLOR);
}

void EngineComp::drawEngineAnalysis() const {

}

void EngineComp::drawEngineControls() const {
    std::string engineControlsText =
        (isEngineRunning) ?
        "Press (x) to stop move analysis" :
        "Press (x) to start move analysis";
    
    DrawText(engineControlsText.c_str(),
        CtrlCfg::START_DRAW_X - 300,
        CtrlCfg::START_DRAW_Y - 50,
        CtrlCfg::FONT_SIZE_PX, CtrlCfg::FONT_COLOR);
}