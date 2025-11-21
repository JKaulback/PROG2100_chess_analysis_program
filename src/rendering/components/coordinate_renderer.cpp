#include "coordinate_renderer.h"
#include "../../config/config.h"
#include "../../application/chess_analysis_program.h"

namespace BoardCfg = Config::Board;

CoordinateRenderer::CoordinateRenderer(const ChessAnalysisProgram& controller) : controller(controller) {
}

void CoordinateRenderer::draw() const {
    drawCoordinates();
}

void CoordinateRenderer::drawCoordinates() const {
    const int fontSize = 24;
    const Color labelColor = {220, 220, 220, 255};  // Light gray for high contrast
    const Color shadowColor = {20, 25, 35, 200};    // Dark shadow matching background
    const Color outlineColor = {40, 50, 65, 255};   // Dark outline for definition
    
    // Draw file letters (a-h) at bottom and top
    for (int file = 0; file < 8; file++) {
        // Choose the correct file letter based on board orientation
        char fileLetter = controller.getBoardFlipped() ? ('h' - file) : ('a' + file);
        const float xPos = BoardCfg::OFFSET_X + (file * BoardCfg::SQUARE_SIZE) + (BoardCfg::SQUARE_SIZE / 2) - (fontSize / 3);
          
        // Top labels with enhanced contrast
        const float topY = BoardCfg::OFFSET_Y - 35;
        // Draw outline for better definition
        DrawText(TextFormat("%c", fileLetter), static_cast<int>(xPos - 1), static_cast<int>(topY), fontSize, outlineColor);
        DrawText(TextFormat("%c", fileLetter), static_cast<int>(xPos + 1), static_cast<int>(topY), fontSize, outlineColor);
        DrawText(TextFormat("%c", fileLetter), static_cast<int>(xPos), static_cast<int>(topY - 1), fontSize, outlineColor);
        DrawText(TextFormat("%c", fileLetter), static_cast<int>(xPos), static_cast<int>(topY + 1), fontSize, outlineColor);
        // Draw shadow
        DrawText(TextFormat("%c", fileLetter), static_cast<int>(xPos + 2), static_cast<int>(topY + 2), fontSize, shadowColor);
        // Draw main text
        DrawText(TextFormat("%c", fileLetter), static_cast<int>(xPos), static_cast<int>(topY), fontSize, labelColor);
    }
    
    // Draw rank numbers (1-8) at left and right
    for (int rank = 0; rank < 8; rank++) {
        // Choose the correct rank number based on board orientation
        int rankNumber = controller.getBoardFlipped() ? (8 - rank) : (rank + 1);
        const float yPos = BoardCfg::OFFSET_Y + ((7 - rank) * BoardCfg::SQUARE_SIZE) + (BoardCfg::SQUARE_SIZE / 2) - (fontSize / 2);
        
        // Left labels with enhanced contrast
        const float leftX = BoardCfg::OFFSET_X - 35;
        // Draw outline for better definition
        DrawText(TextFormat("%d", rankNumber), static_cast<int>(leftX - 1), static_cast<int>(yPos), fontSize, outlineColor);
        DrawText(TextFormat("%d", rankNumber), static_cast<int>(leftX + 1), static_cast<int>(yPos), fontSize, outlineColor);
        DrawText(TextFormat("%d", rankNumber), static_cast<int>(leftX), static_cast<int>(yPos - 1), fontSize, outlineColor);
        DrawText(TextFormat("%d", rankNumber), static_cast<int>(leftX), static_cast<int>(yPos + 1), fontSize, outlineColor);
        // Draw shadow
        DrawText(TextFormat("%d", rankNumber), static_cast<int>(leftX + 2), static_cast<int>(yPos + 2), fontSize, shadowColor);
        // Draw main text
        DrawText(TextFormat("%d", rankNumber), static_cast<int>(leftX), static_cast<int>(yPos), fontSize, labelColor);
    }
}