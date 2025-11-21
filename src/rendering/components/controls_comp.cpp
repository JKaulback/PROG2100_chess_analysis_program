#include "controls_comp.h"
#include "../../config/config.h"

namespace ControlsPanelCfg = Config::ControlsPanel;

ControlsComp::ControlsComp(const ChessAnalysisProgram& controller) :
    controller(controller) {}

void ControlsComp::draw() const {
    drawControlsPanel();
}

Rectangle ControlsComp::getPanelBounds() const {
    // Position below the engine component
    return Rectangle{
        Config::Window::WIDTH - ControlsPanelCfg::PANEL_WIDTH,  // Align to right edge
        Config::StatsPanel::PANEL_HEIGHT + Config::EngineDialog::DIALOG_HEIGHT,  // Position below both StatsPanel and EngineComp
        ControlsPanelCfg::PANEL_WIDTH,
        ControlsPanelCfg::PANEL_HEIGHT
    };
}

void ControlsComp::drawControlsPanel() const {
    Rectangle panelBounds = getPanelBounds();
    
    // Draw main panel background
    DrawRectangle(panelBounds.x, panelBounds.y, panelBounds.width, panelBounds.height, 
                  Color{250, 252, 255, 255});
    
    // Draw left border to separate from main content
    DrawRectangle(panelBounds.x, panelBounds.y, 2, panelBounds.height, 
                  Color{200, 205, 210, 255});
    
    // Draw subtle inner shadow along the left edge
    for (int i = 0; i < 6; i++) {
        unsigned char alpha = static_cast<unsigned char>(15 - (i * 2));
        DrawRectangle(panelBounds.x + 2 + i, panelBounds.y, 1, panelBounds.height, 
                      Color{0, 0, 0, alpha});
    }
    
    // Draw content
    drawPanelTitle();
    
    // Draw control groups with dynamic positioning
    int currentY = panelBounds.y + ControlsPanelCfg::TITLE_HEIGHT + 8 + ControlsPanelCfg::PANEL_PADDING;
    
    // Game Controls
    drawControlGroup("Game Controls:", {
        "Left Click - Select/Move piece",
        "Right Click - Cancel selection",
        "Left Arrow - Undo move",
        "Right Arrow - Redo move",
        "R - Reset board"
    }, currentY);
    
    currentY += 4; // Reduced spacing between groups
    
    // Engine Controls
    drawControlGroup("Engine Controls:", {
        "X - Toggle engine analysis"
    }, currentY);
    
    currentY += 4; // Reduced spacing between groups
    
    // View Controls
    drawControlGroup("View Controls:", {
        "F11 - Toggle fullscreen"
    }, currentY);
}

void ControlsComp::drawPanelTitle() const {
    Rectangle panelBounds = getPanelBounds();
    
    // Draw title bar background
    Rectangle titleRect = {
        panelBounds.x,
        panelBounds.y,
        panelBounds.width,
        ControlsPanelCfg::TITLE_HEIGHT + 4
    };
    DrawRectangle(titleRect.x, titleRect.y, titleRect.width, titleRect.height, 
                  Color{225, 230, 235, 255});
    
    // Draw bottom border of title bar
    DrawRectangle(titleRect.x, titleRect.y + titleRect.height - 1, titleRect.width, 1, 
                  Color{180, 185, 190, 255});
    
    // Draw title text
    const char* title = ControlsPanelCfg::TITLE_TEXT;
    int titleX = panelBounds.x + ControlsPanelCfg::PANEL_PADDING;
    int titleY = panelBounds.y + 12;
    
    drawText(title, titleX, titleY, 22, Color{70, 75, 80, 255});
}

void ControlsComp::drawControlGroup(const std::string& title, const std::vector<std::string>& controls, int& currentY) const {
    Rectangle panelBounds = getPanelBounds();
    
    // Check if we have space for the group title (less restrictive bounds checking)
    if (currentY + ControlsPanelCfg::LINE_HEIGHT > panelBounds.y + panelBounds.height - 10) {
        return; // Not enough space
    }
    
    // Draw group title
    int textX = panelBounds.x + ControlsPanelCfg::PANEL_PADDING;
    drawText(title, textX, currentY, 18, Color{60, 65, 70, 255});
    currentY += ControlsPanelCfg::LINE_HEIGHT;
    
    // Draw control items with indentation
    for (const std::string& control : controls) {
        if (currentY + ControlsPanelCfg::LINE_HEIGHT > panelBounds.y + panelBounds.height - 10) {
            break; // Stop if we run out of space
        }
        drawControlItem(control, currentY);
    }
}

void ControlsComp::drawControlItem(const std::string& text, int& currentY) const {
    Rectangle panelBounds = getPanelBounds();
    
    int textX = panelBounds.x + ControlsPanelCfg::PANEL_PADDING + 16; // Indent control items
    drawText(text, textX, currentY, 16, Color{90, 95, 100, 255});
    currentY += ControlsPanelCfg::LINE_HEIGHT;
}

void ControlsComp::drawText(const std::string& text, int x, int y, int fontSize, Color textColor) const {
    Rectangle panelBounds = getPanelBounds();
    
    // Calculate maximum width available for text
    int maxWidth = static_cast<int>(panelBounds.x + panelBounds.width - x - ControlsPanelCfg::PANEL_PADDING);
    
    // Truncate text if it's too long
    std::string displayText = text;
    while (MeasureText(displayText.c_str(), fontSize) > maxWidth && displayText.length() > 10) {
        displayText = displayText.substr(0, displayText.length() - 4) + "...";
    }
    
    // Draw text with subtle shadow for better readability
    DrawText(displayText.c_str(), x + 1, y + 1, fontSize, Color{0, 0, 0, 30});
    DrawText(displayText.c_str(), x, y, fontSize, textColor);
}