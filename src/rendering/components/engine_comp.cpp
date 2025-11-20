#include "engine_comp.h"
#include "../../analysis_engine/uci_engine.h"

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

    drawTextWithBackground(text, 
                          CtrlCfg::START_DRAW_X, 
                          CtrlCfg::START_DRAW_Y + 60, 
                          CtrlCfg::FONT_SIZE_PX, 
                          CtrlCfg::FONT_COLOR);
}

void EngineComp::drawEngineAnalysis() const {
    if(!isEngineRunning) {
        drawTextWithBackground("Engine not running", 
                              CtrlCfg::START_DRAW_X, 
                              CtrlCfg::START_DRAW_Y + 90, 
                              CtrlCfg::FONT_SIZE_PX, 
                              CtrlCfg::FONT_COLOR);
        return;
    }
    
    EngineAnalysis analysis = controller.pollUCIEngineAnalysis();
    
    // Convert state number to readable text
    std::string stateText;
    switch (static_cast<int>(analysis.state)) {
        case 0: stateText = "Disconnected"; break;
        case 1: stateText = "Connecting"; break;
        case 2: stateText = "Ready"; break;
        case 3: stateText = "Stopping"; break;
        case 4: stateText = "Analyzing"; break;
        case 5: stateText = "Error"; break;
        default: stateText = "Unknown"; break;
    }
    
    // Show engine state
    std::string stateInfo = "Engine State: " + stateText;
    drawTextWithBackground(stateInfo, 
                          CtrlCfg::START_DRAW_X, 
                          CtrlCfg::START_DRAW_Y + 90, 
                          CtrlCfg::FONT_SIZE_PX, 
                          CtrlCfg::FONT_COLOR);
    if (analysis.hasResult) {
        // Draw analysis lines
        for (const AnalysisLine& line : analysis.lines) {
            if (!line.text.empty()) {
                drawAnalysisLine(line.text, line.multipv);
            }
        }
    } else {
        // Show helpful message based on state
        std::string helpText;
        if (analysis.state == EngineState::Ready) {
            helpText = "Ready - Waiting for analysis to start...";
        } else if (analysis.state == EngineState::Analyzing) {
            helpText = "Analyzing - Waiting for results...";
        } else if (analysis.state == EngineState::Connecting) {
            helpText = "Connecting to engine...";
        } else if (analysis.state == EngineState::Error) {
            helpText = "Engine error occurred";
        } else {
            helpText = "No analysis results yet";
        }
        
        drawTextWithBackground(helpText, 
                              CtrlCfg::START_DRAW_X, 
                              CtrlCfg::START_DRAW_Y + 115, 
                              CtrlCfg::FONT_SIZE_PX, 
                              CtrlCfg::FONT_COLOR);
    }
}

void EngineComp::drawAnalysisLine(const std::string text, const int multipv) const {
    // Adjust positioning to avoid overlap and ensure visibility
    int yPos = CtrlCfg::START_DRAW_Y + 125 + (25 * (multipv - 1));
    
    drawTextWithBackground(text, 
                          CtrlCfg::START_DRAW_X, 
                          yPos, 
                          CtrlCfg::FONT_SIZE_PX, 
                          CtrlCfg::FONT_COLOR);
}

void EngineComp::drawEngineControls() const {
    std::string engineControlsText =
        (isEngineRunning) ?
        "Press (x) to stop move analysis" :
        "Press (x) to start move analysis";
    
    drawTextWithBackground(engineControlsText, 
                          CtrlCfg::START_DRAW_X, 
                          CtrlCfg::START_DRAW_Y + 20, 
                          CtrlCfg::FONT_SIZE_PX, 
                          CtrlCfg::FONT_COLOR);
}

void EngineComp::drawTextWithBackground(const std::string& text, int x, int y, int fontSize, Color textColor, Color bgColor) const {
    // Calculate text dimensions
    int textWidth = MeasureText(text.c_str(), fontSize);
    int padding = 8;
    
    Rectangle rect = {
        static_cast<float>(x - padding), 
        static_cast<float>(y - padding), 
        static_cast<float>(textWidth + padding * 2), 
        static_cast<float>(fontSize + padding * 2)
    };
    
    Rectangle shadowRect = {
        rect.x + 3, rect.y + 3, rect.width, rect.height
    };
    
    // Draw shadow for depth effect
    DrawRectangleRounded(shadowRect, 0.2f, 6, Color{0, 0, 0, 50});
    
    // Draw main background with rounded corners
    DrawRectangleRounded(rect, 0.2f, 6, Color{245, 245, 245, 230});
    
    // Draw subtle border using regular rectangle for compatibility
    DrawRectangleLinesEx(rect, 1, Color{160, 160, 160, 180});
    
    // Add inner highlight
    Rectangle innerRect = {rect.x + 1, rect.y + 1, rect.width - 2, rect.height - 2};
    DrawRectangleLinesEx(innerRect, 1, Color{255, 255, 255, 100});
    
    // Draw the text with slight shadow
    DrawText(text.c_str(), x + 1, y + 1, fontSize, Color{0, 0, 0, 30});
    DrawText(text.c_str(), x, y, fontSize, textColor);
}