#include "chess_gui.h"
#include "../application/chess_analysis_program.h"

namespace WinCfg = Config::Window;

ChessGUI::ChessGUI(const ChessAnalysisProgram& controller): 
    controller(controller), boardComp(std::make_unique<BoardComp>(controller)), 
    statsPanel(std::make_unique<StatsPanel>(controller)), 
    engineComp(std::make_unique<EngineComp>(controller)), 
    gameOverlay(std::make_unique<GameOverlay>(controller))
{
    InitWindow(WinCfg::WIDTH, WinCfg::HEIGHT, WinCfg::TITLE);

    SetTargetFPS(WinCfg::TARGET_FPS); // Set the desired frame rate
    
    // Toggle full screen
    ToggleFullscreen();
}

ChessGUI::~ChessGUI() {
    CloseWindow(); // Close window and OpenGL context
}

void ChessGUI::draw() const {
    BeginDrawing();
    ClearBackground(WHITE);
    
    // Draw components in order
    boardComp->draw();
    statsPanel->draw();
    engineComp->draw();
    gameOverlay->draw();  // Last to overlay on top
    
    EndDrawing();
}

// Delegate board methods to BoardRenderer
Vector2 ChessGUI::screenPosToBoardPos(const Vector2 pos) const {
    return boardComp->screenPosToBoardPos(pos);
}

Vector2 ChessGUI::boardPosToScreenPos(const Vector2 pos) const {
    return boardComp->boardPosToScreenPos(pos);
}

float ChessGUI::getSquareSize() const {
    return boardComp->getSquareSize();
}

float ChessGUI::getPieceSize() const {
    return boardComp->getPieceSize();
}

void ChessGUI::setIsUCIEngineRunning(const bool isRunning) {
    engineComp->setEngineRunning(isRunning);
}