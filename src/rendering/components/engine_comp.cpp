#include "engine_comp.h"
#include "../../config/config.h"
#include "../../analysis_engine/uci_engine.h"

namespace EngineDialogCfg = Config::EngineDialog;

EngineComp::EngineComp(const ChessAnalysisProgram& controller) :
    controller(controller) {
    
    isEngineRunning = false;
}

void EngineComp::draw() const {
    drawDialogWindow();
}

void EngineComp::setEngineRunning(bool isRunning) {
    isEngineRunning = isRunning;
}

Rectangle EngineComp::getDialogBounds() const {
    // Position as a built-in panel on the right side
    return Rectangle{
        Config::Window::WIDTH - EngineDialogCfg::DIALOG_WIDTH,  // Align to right edge
        0,  // Start from top edge
        EngineDialogCfg::DIALOG_WIDTH,
        EngineDialogCfg::DIALOG_HEIGHT
    };
}

void EngineComp::drawDialogWindow() const {
    Rectangle panelBounds = getDialogBounds();
    
    // Draw main panel background with subtle gradient effect
    DrawRectangle(panelBounds.x, panelBounds.y, panelBounds.width, panelBounds.height, 
                  Color{240, 242, 245, 255});
    
    // Draw left border to separate from main content
    DrawRectangle(panelBounds.x, panelBounds.y, 2, panelBounds.height, 
                  Color{200, 205, 210, 255});
    
    // Draw subtle inner shadow along the left edge
    for (int i = 0; i < 8; i++) {
        unsigned char alpha = static_cast<unsigned char>(20 - (i * 2));
        DrawRectangle(panelBounds.x + 2 + i, panelBounds.y, 1, panelBounds.height, 
                      Color{0, 0, 0, alpha});
    }
    
    // Draw content
    drawDialogTitle();
    drawEngineControls();
    drawEngineStatus();
    drawEngineAnalysis();
}

void EngineComp::drawDialogTitle() const {
    Rectangle panelBounds = getDialogBounds();
    
    // Draw title bar background
    Rectangle titleRect = {
        panelBounds.x,
        panelBounds.y,
        panelBounds.width,
        EngineDialogCfg::TITLE_HEIGHT + 4
    };
    DrawRectangle(titleRect.x, titleRect.y, titleRect.width, titleRect.height, 
                  Color{220, 225, 230, 255});
    
    // Draw bottom border of title bar
    DrawRectangle(titleRect.x, titleRect.y + titleRect.height - 1, titleRect.width, 1, 
                  Color{180, 185, 190, 255});
    
    // Draw title text
    const char* title = EngineDialogCfg::TITLE_TEXT;
    int titleWidth = MeasureText(title, 22);
    int titleX = panelBounds.x + EngineDialogCfg::DIALOG_PADDING;
    int titleY = panelBounds.y + 12;
    
    drawText(title, titleX, titleY, 22, Color{70, 75, 80, 255});
}

void EngineComp::drawEngineControls() const {
    Rectangle panelBounds = getDialogBounds();
    
    std::string controlText = isEngineRunning ? 
        "Press (X) to stop analysis" : 
        "Press (X) to start analysis";
    
    int textX = panelBounds.x + EngineDialogCfg::DIALOG_PADDING;
    int textY = panelBounds.y + EngineDialogCfg::TITLE_HEIGHT + 8 + EngineDialogCfg::DIALOG_PADDING;
    
    drawText(controlText, textX, textY, 18, Color{90, 95, 100, 255});
}

void EngineComp::drawEngineStatus() const {
    Rectangle panelBounds = getDialogBounds();
    
    std::string statusText = isEngineRunning ? 
        "Engine Running" : 
        "Engine Stopped";
    
    Color statusColor = isEngineRunning ? 
        Color{46, 160, 67, 255} :  // Success green
        Color{220, 53, 69, 255};   // Danger red
    
    int textX = panelBounds.x + EngineDialogCfg::DIALOG_PADDING;
    int textY = panelBounds.y + EngineDialogCfg::TITLE_HEIGHT + 8 + EngineDialogCfg::DIALOG_PADDING + EngineDialogCfg::LINE_HEIGHT + 8;
    
    drawText(statusText, textX, textY, 18, statusColor);
}

void EngineComp::drawEngineAnalysis() const {
    Rectangle panelBounds = getDialogBounds();
    
    if (!isEngineRunning) {
        int textX = panelBounds.x + EngineDialogCfg::DIALOG_PADDING;
        int textY = panelBounds.y + EngineDialogCfg::TITLE_HEIGHT + 8 + EngineDialogCfg::DIALOG_PADDING + (EngineDialogCfg::LINE_HEIGHT * 3) + 10;
        
        drawText("Engine is not running.", textX, textY, 18, Color{128, 128, 128, 255});
        drawText("Start the engine to see analysis.", textX, textY + EngineDialogCfg::LINE_HEIGHT, 18, Color{128, 128, 128, 255});
        return;
    }
    
    EngineAnalysis analysis = controller.pollUCIEngineAnalysis();
    
    // Draw analysis header
    int textX = panelBounds.x + EngineDialogCfg::DIALOG_PADDING;
    int currentY = panelBounds.y + EngineDialogCfg::TITLE_HEIGHT + 8 + EngineDialogCfg::DIALOG_PADDING + (EngineDialogCfg::LINE_HEIGHT * 3) + 10;
    
    // Convert state to readable text
    std::string stateText;
    Color stateColor = Color{100, 100, 100, 255};
    
    switch (static_cast<int>(analysis.state)) {
        case 0: stateText = "Disconnected"; stateColor = Color{178, 34, 34, 255}; break;
        case 1: stateText = "Connecting"; stateColor = Color{255, 140, 0, 255}; break;
        case 2: stateText = "Ready"; stateColor = Color{34, 139, 34, 255}; break;
        case 3: stateText = "Stopping"; stateColor = Color{255, 140, 0, 255}; break;
        case 4: stateText = "Analyzing"; stateColor = Color{0, 100, 200, 255}; break;
        case 5: stateText = "Error"; stateColor = Color{178, 34, 34, 255}; break;
        default: stateText = "Unknown"; break;
    }
    
    std::string engineStateText = "Engine State: " + stateText;
    drawText(engineStateText, textX, currentY, 19, stateColor);
    currentY += EngineDialogCfg::LINE_HEIGHT + 5;
    DrawLine(textX, currentY, panelBounds.x + panelBounds.width - EngineDialogCfg::DIALOG_PADDING, currentY,
             Color{200, 200, 200, 255});
    currentY += 10;
    
    if (analysis.hasResult && !analysis.lines.empty()) {
        drawText("Analysis Results:", textX, currentY, 19, Color{60, 60, 60, 255});
        currentY += EngineDialogCfg::LINE_HEIGHT + 5;
        
        // Draw analysis lines
        for (const AnalysisLine& line : analysis.lines) {
            if (!line.text.empty() && currentY < panelBounds.y + panelBounds.height - EngineDialogCfg::DIALOG_PADDING) {
                drawAnalysisLine(line.text, line.multipv, currentY);
            }
        }
    } else {
        // Show helpful message based on state
        std::string helpText;
        if (analysis.state == EngineState::Ready) {
            helpText = "Ready - Waiting for analysis...";
        } else if (analysis.state == EngineState::Analyzing) {
            helpText = "Analyzing position...";
        } else if (analysis.state == EngineState::Connecting) {
            helpText = "Connecting to engine...";
        } else if (analysis.state == EngineState::Error) {
            helpText = "Engine error occurred";
        } else {
            helpText = "No analysis results available";
        }
        
        drawText(helpText, textX, currentY, 18, Color{128, 128, 128, 255});
    }
}

void EngineComp::drawAnalysisLine(const std::string& text, const int multipv, int& currentY) const {
    Rectangle panelBounds = getDialogBounds();
    
    int textX = panelBounds.x + EngineDialogCfg::DIALOG_PADDING + 8; // Slight indent for analysis lines
    
    // Truncate text if it's too long to fit in the panel
    std::string displayText = text;
    int maxWidth = EngineDialogCfg::DIALOG_WIDTH - (EngineDialogCfg::DIALOG_PADDING * 2) - 16;
    
    while (MeasureText(displayText.c_str(), 16) > maxWidth && displayText.length() > 10) {
        displayText = displayText.substr(0, displayText.length() - 4) + "...";
    }
    
    // Use different colors for different multipv lines
    Color lineColor;
    switch (multipv) {
        case 1: lineColor = Color{13, 110, 253, 255}; break;  // Primary blue
        case 2: lineColor = Color{25, 135, 84, 255}; break;   // Success green
        case 3: lineColor = Color{255, 193, 7, 255}; break;   // Warning yellow
        default: lineColor = Color{108, 117, 125, 255}; break; // Secondary gray
    }
    
    drawText(displayText, textX, currentY, 16, lineColor);
    currentY += EngineDialogCfg::LINE_HEIGHT;
}

void EngineComp::drawText(const std::string& text, int x, int y, int fontSize, Color textColor) const {
    // Draw text with subtle shadow for better readability
    DrawText(text.c_str(), x + 1, y + 1, fontSize, Color{0, 0, 0, 30});
    DrawText(text.c_str(), x, y, fontSize, textColor);
}