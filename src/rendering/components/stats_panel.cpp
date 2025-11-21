#include "stats_panel.h"
#include "../../config/config.h"

namespace StatsPanelCfg = Config::StatsPanel;

StatsPanel::StatsPanel(const ChessAnalysisProgram& controller) :
    controller(controller) {}

void StatsPanel::draw() const {
    drawStatsPanel();
}

Rectangle StatsPanel::getPanelBounds() const {
    // Position below the engine analysis panel
    return Rectangle{
        Config::Window::WIDTH - StatsPanelCfg::PANEL_WIDTH,  // Align to right edge
        350,  // Position below engine analysis (which ends around 350px)
        StatsPanelCfg::PANEL_WIDTH,
        StatsPanelCfg::PANEL_HEIGHT
    };
}

void StatsPanel::drawStatsPanel() const {
    Rectangle panelBounds = getPanelBounds();
    
    // Draw main panel background
    DrawRectangle(panelBounds.x, panelBounds.y, panelBounds.width, panelBounds.height, 
                  Color{245, 247, 250, 255});
    
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
    
    // Draw statistics with dynamic positioning
    int currentY = panelBounds.y + StatsPanelCfg::TITLE_HEIGHT + 8 + StatsPanelCfg::PANEL_PADDING;
    drawCurrentPlayer(currentY);
    drawHalfMoveClock(currentY);
    drawGameStatus(currentY);
    drawCapturedPieces(currentY);
}

void StatsPanel::drawPanelTitle() const {
    Rectangle panelBounds = getPanelBounds();
    
    // Draw title bar background
    Rectangle titleRect = {
        panelBounds.x,
        panelBounds.y,
        panelBounds.width,
        StatsPanelCfg::TITLE_HEIGHT + 4
    };
    DrawRectangle(titleRect.x, titleRect.y, titleRect.width, titleRect.height, 
                  Color{225, 230, 235, 255});
    
    // Draw bottom border of title bar
    DrawRectangle(titleRect.x, titleRect.y + titleRect.height - 1, titleRect.width, 1, 
                  Color{180, 185, 190, 255});
    
    // Draw title text
    const char* title = StatsPanelCfg::TITLE_TEXT;
    int titleX = panelBounds.x + StatsPanelCfg::PANEL_PADDING;
    int titleY = panelBounds.y + 10;
    
    drawText(title, titleX, titleY, 20, Color{70, 75, 80, 255});
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
    // Draw text with subtle shadow for better readability
    DrawText(text.c_str(), x + 1, y + 1, fontSize, Color{0, 0, 0, 20});
    DrawText(text.c_str(), x, y, fontSize, textColor);
}
