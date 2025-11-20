#include "game_overlay.h"

namespace GOCfg = Config::GameOver;

GameOverlay::GameOverlay(const ChessAnalysisProgram& controller) :
    controller(controller) {}

void GameOverlay::draw() const {
    // Draw controls
    void drawControls();

    // Check for game over
    if (controller.isGameOver())
        drawGameOverScreen();
}

void GameOverlay::drawGameOverScreen() const {
    // Get the text to display
    std::string gameOverText = controller.getGameOverString();

    // Calculate text size
    Vector2 textSize = MeasureTextEx(GetFontDefault(), gameOverText.c_str(), 
        GOCfg::STATE_FONT_SIZE_PX, GOCfg::STATE_FONT_SPACING);

    // Center text position in window
    Vector2 textPos = {
        (GetScreenWidth() / 2.0f) - (textSize.x / 2.0f),
        (GetScreenHeight() / 2.0f) - (textSize.y / 2.0f)
    };

    // Draw game over background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GOCfg::BACKGROUND_COLOR);

    // Draw text
    DrawText(gameOverText.c_str(), (int)textPos.x, (int)textPos.y, 
        GOCfg::STATE_FONT_SIZE_PX, GOCfg::STATE_FONT_COLOR);
}

void GameOverlay::drawControls() const {
    // TODO: Add controls
}