#include "stats_panel.h"
#include "ui_renderer.h"
#include "../../config/config.h"

namespace StatsPanelCfg = Config::StatsPanel;

StatsPanel::StatsPanel(const ChessAnalysisProgram& controller) :
    controller(controller) {}

void StatsPanel::draw() const {
    drawStatsPanel();
}

Rectangle StatsPanel::getPanelBounds() const {
    // Position at the top of the vertically centered left panel area
    float totalPanelHeight = StatsPanelCfg::PANEL_HEIGHT + Config::EngineDialog::DIALOG_HEIGHT + Config::ControlsPanel::PANEL_HEIGHT;
    float verticalCenterOffset = (Config::Window::HEIGHT - totalPanelHeight) / 2.0f;
    return Rectangle{
        0,  // Align to left edge
        verticalCenterOffset,  // Start from vertically centered position
        StatsPanelCfg::PANEL_WIDTH,
        StatsPanelCfg::PANEL_HEIGHT
    };
}

void StatsPanel::drawStatsPanel() const {
    Rectangle panelBounds = getPanelBounds();
    
    // Draw panel using UIRenderer
    UIRenderer::drawPanelBackground(panelBounds, UIRenderer::PanelStyle::Stats);
    UIRenderer::drawPanelBorder(panelBounds);
    UIRenderer::drawPanelShadowRight(panelBounds, 6);
    
    // Draw content
    drawPanelTitle();
    
    // Draw statistics with dynamic positioning
    int currentY = panelBounds.y + StatsPanelCfg::TITLE_HEIGHT + 8 + StatsPanelCfg::PANEL_PADDING;
    drawCurrentPlayer(currentY);
    drawHalfMoveClock(currentY);
    drawGameStatus(currentY);
    drawCapturedPieces(currentY);
}

void StatsPanel::drawPanelTitle() const {
    Rectangle panelBounds = getPanelBounds();
    UIRenderer::drawPanelTitle(panelBounds, StatsPanelCfg::TITLE_TEXT, 
                              StatsPanelCfg::TITLE_HEIGHT, StatsPanelCfg::PANEL_PADDING);
}

void StatsPanel::drawStat(const std::string& label, const std::string& value, int& currentY) const {
    Rectangle panelBounds = getPanelBounds();
    
    int labelX = panelBounds.x + StatsPanelCfg::PANEL_PADDING;
    int valueX = panelBounds.x + StatsPanelCfg::PANEL_PADDING + 140; // Fixed offset for alignment
    
    // Draw label
    drawText(label, labelX, currentY, 17, Color{90, 95, 100, 255});
    
    // Draw value
    drawText(value, valueX, currentY, 17, Color{40, 45, 50, 255});
    
    currentY += StatsPanelCfg::LINE_HEIGHT + 4;
}

void StatsPanel::drawCurrentPlayer(int& currentY) const {
    char player = controller.getCurrentPlayer();
    std::string playerName = (player == 'w') ? "White" : "Black";
    Color playerColor = (player == 'w') ? Color{100, 100, 100, 255} : Color{60, 60, 60, 255};
    
    Rectangle panelBounds = getPanelBounds();
    int labelX = panelBounds.x + StatsPanelCfg::PANEL_PADDING;
    int valueX = panelBounds.x + StatsPanelCfg::PANEL_PADDING + 140;
    
    drawText("Current Player:", labelX, currentY, 17, Color{90, 95, 100, 255});
    drawText(playerName, valueX, currentY, 17, playerColor);
    
    currentY += StatsPanelCfg::LINE_HEIGHT + 4;
}

void StatsPanel::drawHalfMoveClock(int& currentY) const {
    std::string clockValue = std::to_string(controller.getHalfmoveClock());
    drawStat("Halfmove Clock:", clockValue, currentY);
}

void StatsPanel::drawGameStatus(int& currentY) const {
    std::string status;
    Color statusColor;
    
    if (controller.isGameOver()) {
        status = "Game Over";
        statusColor = Color{220, 53, 69, 255}; // Danger red
    } else {
        status = "In Progress";
        statusColor = Color{46, 160, 67, 255}; // Success green
    }
    
    Rectangle panelBounds = getPanelBounds();
    int labelX = panelBounds.x + StatsPanelCfg::PANEL_PADDING;
    int valueX = panelBounds.x + StatsPanelCfg::PANEL_PADDING + 140;
    
    drawText("Game Status:", labelX, currentY, 17, Color{90, 95, 100, 255});
    drawText(status, valueX, currentY, 17, statusColor);
    
    currentY += StatsPanelCfg::LINE_HEIGHT + 4;
}

void StatsPanel::drawCapturedPieces(int& currentY) const {
    std::vector<char> captured = controller.getCapturedPieces();
    std::string capturedCount = std::to_string(captured.size());
    drawStat("Captured Pieces:", capturedCount, currentY);
}

void StatsPanel::drawText(const std::string& text, int x, int y, int fontSize, Color textColor) const {
    UIRenderer::drawTextWithShadow(text, x, y, fontSize, textColor);
}
