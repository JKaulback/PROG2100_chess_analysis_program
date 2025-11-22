#include "controls_comp.h"
#include "ui_renderer.h"
#include "../../config/config.h"

namespace ControlsPanelCfg = Config::ControlsPanel;

ControlsComp::ControlsComp(const ChessAnalysisProgram& controller) :
    controller(controller) {}

void ControlsComp::draw() const {
    drawControlsPanel();
}

Rectangle ControlsComp::getPanelBounds() const {
    // Position below the engine component
    float totalPanelHeight = Config::StatsPanel::PANEL_HEIGHT + Config::EngineDialog::DIALOG_HEIGHT + ControlsPanelCfg::PANEL_HEIGHT;
    float verticalCenterOffset = (Config::Window::HEIGHT - totalPanelHeight) / 2.0f;
    return Rectangle{
        0,  // Align to left edge
        verticalCenterOffset + Config::StatsPanel::PANEL_HEIGHT + Config::EngineDialog::DIALOG_HEIGHT,  // Position below both StatsPanel and EngineComp
        ControlsPanelCfg::PANEL_WIDTH,
        ControlsPanelCfg::PANEL_HEIGHT
    };
}

void ControlsComp::drawControlsPanel() const {
    Rectangle panelBounds = getPanelBounds();
    
    // Draw panel using UIRenderer
    UIRenderer::drawPanelBackground(panelBounds, UIRenderer::PanelStyle::Controls);
    UIRenderer::drawPanelBorder(panelBounds);
    UIRenderer::drawPanelShadowRight(panelBounds, 6);
    
    // Draw content
    drawPanelTitle();
    
    // Draw control groups with dynamic positioning
    int currentY = panelBounds.y + ControlsPanelCfg::TITLE_HEIGHT + 8 + ControlsPanelCfg::PANEL_PADDING;
    
    // Game Controls
    drawControlGroup("Game Controls:", {
        "Left Click - Drag and drop piece",
        "Left Arrow - Undo move",
        "Right Arrow - Redo move",
        "F - Flip board",
        "R - Reset board"
    }, currentY);
    
    currentY += 4; // Reduced spacing between groups
    
    // Engine Controls
    drawControlGroup("Engine Controls:", {
        "X - Toggle engine analysis"
    }, currentY);
    
}

void ControlsComp::drawPanelTitle() const {
    Rectangle panelBounds = getPanelBounds();
    UIRenderer::drawPanelTitle(panelBounds, ControlsPanelCfg::TITLE_TEXT, 
                              ControlsPanelCfg::TITLE_HEIGHT, ControlsPanelCfg::PANEL_PADDING);
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
    
    // Use UIRenderer for text with shadow
    UIRenderer::drawTextWithShadow(displayText, x, y, fontSize, textColor);
}