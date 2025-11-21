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
    
    // Draw modern background first
    drawModernBackground();
    
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

void ChessGUI::drawModernBackground() const {
    // Get window dimensions from config
    const int windowWidth = WinCfg::WIDTH;
    const int windowHeight = WinCfg::HEIGHT;
    
    // Modern gradient background (dark blue to darker blue)
    const Color topColor = {25, 30, 45, 255};     // Dark blue-gray
    const Color bottomColor = {15, 20, 35, 255};  // Darker blue-gray
    
    // Draw gradient background
    DrawRectangleGradientV(0, 0, windowWidth, windowHeight, topColor, bottomColor);
    
    // Add subtle geometric pattern overlay
    drawGeometricPattern();
    
    // Add ambient lighting effect around board area
    drawAmbientLighting();
}

void ChessGUI::drawGeometricPattern() const {
    const Color patternColor = {35, 40, 55, 40}; // Very subtle overlay
    const int gridSize = 80;
    const int windowWidth = WinCfg::WIDTH;
    const int windowHeight = WinCfg::HEIGHT;
    
    // Draw subtle diagonal lines pattern
    for (int x = 0; x < windowWidth + gridSize; x += gridSize) {
        for (int y = 0; y < windowHeight + gridSize; y += gridSize) {
            // Draw subtle diamonds/squares rotated 45 degrees
            Vector2 points[4] = {
                {static_cast<float>(x), static_cast<float>(y - 20)},
                {static_cast<float>(x + 20), static_cast<float>(y)},
                {static_cast<float>(x), static_cast<float>(y + 20)},
                {static_cast<float>(x - 20), static_cast<float>(y)}
            };
            
            // Only draw if not overlapping with board area
            if (x < Config::Board::OFFSET_X - 50 || x > Config::Board::OFFSET_X + Config::Board::SIZE + 50 ||
                y < Config::Board::OFFSET_Y - 50 || y > Config::Board::OFFSET_Y + Config::Board::SIZE + 50) {
                DrawTriangle(points[0], points[1], points[2], patternColor);
                DrawTriangle(points[0], points[2], points[3], patternColor);
            }
        }
    }
}

void ChessGUI::drawAmbientLighting() const {
    // Create soft glow effect around the board area
    const float boardCenterX = Config::Board::OFFSET_X + Config::Board::SIZE / 2;
    const float boardCenterY = Config::Board::OFFSET_Y + Config::Board::SIZE / 2;
    const float glowRadius = Config::Board::SIZE * 0.8f;
    
    // Draw multiple concentric circles for gradient glow effect
    const int glowLayers = 8;
    for (int i = glowLayers; i > 0; i--) {
        float currentRadius = glowRadius * (static_cast<float>(i) / glowLayers);
        int alpha = 15 - (i * 2); // Fade from center outward
        if (alpha > 0) {
            Color glowColor = {60, 80, 120, static_cast<unsigned char>(alpha)};
            DrawCircleGradient(
                static_cast<int>(boardCenterX),
                static_cast<int>(boardCenterY),
                currentRadius,
                glowColor,
                {0, 0, 0, 0} // Transparent edge
            );
        }
    }
}