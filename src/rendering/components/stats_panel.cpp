#include "stats_panel.h"
#include "../../config/config.h"

namespace PieceCfg = Config::Pieces;
namespace StatsCfg = Config::GameStatistics;

StatsPanel::StatsPanel(const ChessAnalysisProgram& controller) :
    controller(controller) {}

void StatsPanel::draw() const {
    // Draw title panel
    drawTextWithBackground("GAME STATISTICS", 
                          StatsCfg::START_DRAW_X, 
                          StatsCfg::START_DRAW_Y - 30, 
                          StatsCfg::STATS_FONT_SIZE_PX + 2, 
                          Color{60, 80, 120, 255}, 
                          Color{200, 220, 240, 220});
    
    drawCurrentPlayer(0);
    drawHalfMoveClock(1);
    drawGameStatus(2);
    drawCapturedPieces(3);
}

void StatsPanel::drawStat(const std::string text, const int statIndex) const {
    drawTextWithBackground(text, 
                          StatsCfg::START_DRAW_X, 
                          StatsCfg::START_DRAW_Y + (StatsCfg::DRAW_STEP_Y * statIndex) + 10, 
                          StatsCfg::STATS_FONT_SIZE_PX, 
                          StatsCfg::STATS_FONT_COLOR);
}

void StatsPanel::drawCurrentPlayer(const int statIndex) const {
    drawStat(TextFormat("CURRENT PLAYER: %c", controller.getCurrentPlayer()), statIndex);
}

void StatsPanel::drawHalfMoveClock(const int statIndex) const {
    drawStat(TextFormat("HALFMOVE CLOCK: %i", controller.getHalfmoveClock()), statIndex);
}

void StatsPanel::drawGameStatus(const int statIndex) const {
    std::string statusText = "GAME STATUS: ";
    if (controller.isGameOver()) {
        statusText += "GAME OVER";
    } else {
        statusText += "IN PROGRESS";
    }
    drawStat(statusText, statIndex);
}

void StatsPanel::drawCapturedPieces(const int statIndex) const {
    std::vector<char> captured = controller.getCapturedPieces();
    std::string capturedText = TextFormat("CAPTURED PIECES: %i", static_cast<int>(captured.size()));
    drawStat(capturedText, statIndex);
}

void StatsPanel::drawTextWithBackground(const std::string& text, int x, int y, int fontSize, Color textColor, Color bgColor) const {
    // Calculate text dimensions
    int textWidth = MeasureText(text.c_str(), fontSize);
    int padding = 10;
    
    Rectangle rect = {
        static_cast<float>(x - padding), 
        static_cast<float>(y - padding), 
        static_cast<float>(textWidth + padding * 2), 
        static_cast<float>(fontSize + padding * 2)
    };
    
    Rectangle shadowRect = {
        rect.x + 2, rect.y + 2, rect.width, rect.height
    };
    
    // Draw shadow for depth effect
    DrawRectangleRounded(shadowRect, 0.15f, 6, Color{0, 0, 0, 40});
    
    // Draw main background with rounded corners
    DrawRectangleRounded(rect, 0.15f, 6, bgColor);
    
    // Draw subtle border
    DrawRectangleLinesEx(rect, 1, Color{150, 170, 190, 120});
    
    // Add inner highlight for polish
    Rectangle innerRect = {rect.x + 1, rect.y + 1, rect.width - 2, rect.height - 2};
    DrawRectangleLinesEx(innerRect, 0.5f, Color{255, 255, 255, 80});
    
    // Draw the text with slight shadow
    DrawText(text.c_str(), x + 1, y + 1, fontSize, Color{0, 0, 0, 20});
    DrawText(text.c_str(), x, y, fontSize, textColor);
}
