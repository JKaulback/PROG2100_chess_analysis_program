#include "coordinate_renderer.h"
#include "../../config/config.h"

namespace BoardCfg = Config::Board;

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
        char fileLetter = 'a' + file;
        const float xPos = BoardCfg::OFFSET_X + (file * BoardCfg::SQUARE_SIZE) + (BoardCfg::SQUARE_SIZE / 2) - (fontSize / 3);
        
        // Bottom labels with enhanced contrast
        const float bottomY = BoardCfg::OFFSET_Y + BoardCfg::SIZE + 15;
        // Draw outline for better definition
        DrawText(TextFormat("%c", fileLetter), static_cast<int>(xPos - 1), static_cast<int>(bottomY), fontSize, outlineColor);
        DrawText(TextFormat("%c", fileLetter), static_cast<int>(xPos + 1), static_cast<int>(bottomY), fontSize, outlineColor);
        DrawText(TextFormat("%c", fileLetter), static_cast<int>(xPos), static_cast<int>(bottomY - 1), fontSize, outlineColor);
        DrawText(TextFormat("%c", fileLetter), static_cast<int>(xPos), static_cast<int>(bottomY + 1), fontSize, outlineColor);
        // Draw shadow
        DrawText(TextFormat("%c", fileLetter), static_cast<int>(xPos + 2), static_cast<int>(bottomY + 2), fontSize, shadowColor);
        // Draw main text
        DrawText(TextFormat("%c", fileLetter), static_cast<int>(xPos), static_cast<int>(bottomY), fontSize, labelColor);
        
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
        int rankNumber = rank + 1;
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
        
        // Right labels with enhanced contrast
        const float rightX = BoardCfg::OFFSET_X + BoardCfg::SIZE + 15;
        // Draw outline for better definition
        DrawText(TextFormat("%d", rankNumber), static_cast<int>(rightX - 1), static_cast<int>(yPos), fontSize, outlineColor);
        DrawText(TextFormat("%d", rankNumber), static_cast<int>(rightX + 1), static_cast<int>(yPos), fontSize, outlineColor);
        DrawText(TextFormat("%d", rankNumber), static_cast<int>(rightX), static_cast<int>(yPos - 1), fontSize, outlineColor);
        DrawText(TextFormat("%d", rankNumber), static_cast<int>(rightX), static_cast<int>(yPos + 1), fontSize, outlineColor);
        // Draw shadow
        DrawText(TextFormat("%d", rankNumber), static_cast<int>(rightX + 2), static_cast<int>(yPos + 2), fontSize, shadowColor);
        // Draw main text
        DrawText(TextFormat("%d", rankNumber), static_cast<int>(rightX), static_cast<int>(yPos), fontSize, labelColor);
    }
}