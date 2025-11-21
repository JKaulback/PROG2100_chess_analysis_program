#include "board_renderer.h"
#include "texture_manager.h"
#include "../../config/config.h"

namespace BoardCfg = Config::Board;

BoardRenderer::BoardRenderer(const TextureManager& textureManager) :
    textureManager(textureManager) {
}

void BoardRenderer::draw() const {
    drawBoard();
}

void BoardRenderer::drawBoard() const {
    // Draw fancy border behind the board
    drawBoardBorder();
    
    // Draw the chess board with proper offset for margins
    DrawTextureEx(textureManager.getBoardTexture(), 
        {BoardCfg::OFFSET_X, BoardCfg::OFFSET_Y}, 0.0f, BoardCfg::SCALE, WHITE);
}

void BoardRenderer::drawBoardBorder() const {
    const float borderWidth = 8.0f;
    
    // Calculate border rectangle (slightly larger than board)
    const float borderX = BoardCfg::OFFSET_X - borderWidth;
    const float borderY = BoardCfg::OFFSET_Y - borderWidth;
    const float borderSize = BoardCfg::SIZE + (borderWidth * 2);
    
    // Draw all border layers efficiently
    drawBorderLayers(borderX, borderY, borderSize);
    drawBorderHighlights(borderX, borderY, borderSize);
    drawCornerDecorations(borderX, borderY, borderSize);
}

void BoardRenderer::drawBorderLayers(float borderX, float borderY, float borderSize) const {
    const float shadowOffset = 6.0f;
    const Color borderColor = {139, 69, 19, 255};      // Saddle brown - elegant wood color
    const Color innerBorder = {160, 82, 45, 255};      // Lighter brown accent
    const Color shadowColor = {0, 0, 0, 60};           // Subtle shadow
    const float accentWidth = 2.0f;
    
    // Draw shadow (offset behind the border)
    DrawRectangle(
        static_cast<int>(borderX + shadowOffset), 
        static_cast<int>(borderY + shadowOffset), 
        static_cast<int>(borderSize), 
        static_cast<int>(borderSize), 
        shadowColor
    );
    
    // Draw main border (dark brown)
    DrawRectangle(
        static_cast<int>(borderX), 
        static_cast<int>(borderY), 
        static_cast<int>(borderSize), 
        static_cast<int>(borderSize), 
        borderColor
    );
    
    // Draw inner accent border (lighter brown)
    DrawRectangle(
        static_cast<int>(borderX + accentWidth), 
        static_cast<int>(borderY + accentWidth), 
        static_cast<int>(borderSize - accentWidth * 2), 
        static_cast<int>(borderSize - accentWidth * 2), 
        innerBorder
    );
}

void BoardRenderer::drawBorderHighlights(float borderX, float borderY, float borderSize) const {
    const Color highlightColor = {205, 133, 63, 255};  // Peru - lighter highlight
    const float accentWidth = 2.0f;
    const float highlightWidth = 1.0f;
    
    // Top highlight
    DrawRectangle(
        static_cast<int>(borderX + accentWidth), 
        static_cast<int>(borderY + accentWidth), 
        static_cast<int>(borderSize - accentWidth * 2), 
        static_cast<int>(highlightWidth), 
        highlightColor
    );
    
    // Left highlight
    DrawRectangle(
        static_cast<int>(borderX + accentWidth), 
        static_cast<int>(borderY + accentWidth), 
        static_cast<int>(highlightWidth), 
        static_cast<int>(borderSize - accentWidth * 2), 
        highlightColor
    );
}

void BoardRenderer::drawSingleCornerDecoration(float x, float y, float cornerSize, const Color& decorColor, const Color& accentColor) const {
    DrawRectangle(
        static_cast<int>(x), 
        static_cast<int>(y), 
        static_cast<int>(cornerSize), 
        static_cast<int>(cornerSize), 
        decorColor
    );
    DrawRectangleLines(
        static_cast<int>(x), 
        static_cast<int>(y), 
        static_cast<int>(cornerSize), 
        static_cast<int>(cornerSize), 
        accentColor
    );
}

void BoardRenderer::drawCornerDecorations(float borderX, float borderY, float borderSize) const {
    const float cornerSize = 16.0f;
    const float cornerOffset = 4.0f;
    const float cornerInset = 12.0f;
    const Color decorColor = {205, 133, 63, 180};  // Semi-transparent peru
    const Color accentColor = {139, 69, 19, 255};  // Saddle brown
    
    // Draw all four corners using the helper method
    drawSingleCornerDecoration(borderX - cornerOffset, borderY - cornerOffset, cornerSize, decorColor, accentColor);  // Top-left
    drawSingleCornerDecoration(borderX + borderSize - cornerInset, borderY - cornerOffset, cornerSize, decorColor, accentColor);  // Top-right
    drawSingleCornerDecoration(borderX - cornerOffset, borderY + borderSize - cornerInset, cornerSize, decorColor, accentColor);  // Bottom-left
    drawSingleCornerDecoration(borderX + borderSize - cornerInset, borderY + borderSize - cornerInset, cornerSize, decorColor, accentColor);  // Bottom-right
}