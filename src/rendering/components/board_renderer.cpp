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
    const float shadowOffset = 6.0f;
    const Color borderColor = {139, 69, 19, 255};      // Saddle brown - elegant wood color
    const Color innerBorder = {160, 82, 45, 255};      // Lighter brown accent
    const Color shadowColor = {0, 0, 0, 60};           // Subtle shadow
    const Color highlightColor = {205, 133, 63, 255};  // Peru - lighter highlight
    
    // Calculate border rectangle (slightly larger than board)
    const float borderX = BoardCfg::OFFSET_X - borderWidth;
    const float borderY = BoardCfg::OFFSET_Y - borderWidth;
    const float borderSize = BoardCfg::SIZE + (borderWidth * 2);
    
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
    const float accentWidth = 2.0f;
    DrawRectangle(
        static_cast<int>(borderX + accentWidth), 
        static_cast<int>(borderY + accentWidth), 
        static_cast<int>(borderSize - accentWidth * 2), 
        static_cast<int>(borderSize - accentWidth * 2), 
        innerBorder
    );
    
    // Draw highlight border (top and left edges for 3D effect)
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
    
    // Draw corner decorations
    drawCornerDecorations(borderX, borderY, borderSize);
}

void BoardRenderer::drawCornerDecorations(float borderX, float borderY, float borderSize) const {
    const float cornerSize = 16.0f;
    const Color decorColor = {205, 133, 63, 180};  // Semi-transparent peru
    const Color accentColor = {139, 69, 19, 255};  // Saddle brown
    
    // Top-left corner
    DrawRectangle(
        static_cast<int>(borderX - 4), 
        static_cast<int>(borderY - 4), 
        static_cast<int>(cornerSize), 
        static_cast<int>(cornerSize), 
        decorColor
    );
    DrawRectangleLines(
        static_cast<int>(borderX - 4), 
        static_cast<int>(borderY - 4), 
        static_cast<int>(cornerSize), 
        static_cast<int>(cornerSize), 
        accentColor
    );
    
    // Top-right corner
    DrawRectangle(
        static_cast<int>(borderX + borderSize - 12), 
        static_cast<int>(borderY - 4), 
        static_cast<int>(cornerSize), 
        static_cast<int>(cornerSize), 
        decorColor
    );
    DrawRectangleLines(
        static_cast<int>(borderX + borderSize - 12), 
        static_cast<int>(borderY - 4), 
        static_cast<int>(cornerSize), 
        static_cast<int>(cornerSize), 
        accentColor
    );
    
    // Bottom-left corner
    DrawRectangle(
        static_cast<int>(borderX - 4), 
        static_cast<int>(borderY + borderSize - 12), 
        static_cast<int>(cornerSize), 
        static_cast<int>(cornerSize), 
        decorColor
    );
    DrawRectangleLines(
        static_cast<int>(borderX - 4), 
        static_cast<int>(borderY + borderSize - 12), 
        static_cast<int>(cornerSize), 
        static_cast<int>(cornerSize), 
        accentColor
    );
    
    // Bottom-right corner
    DrawRectangle(
        static_cast<int>(borderX + borderSize - 12), 
        static_cast<int>(borderY + borderSize - 12), 
        static_cast<int>(cornerSize), 
        static_cast<int>(cornerSize), 
        decorColor
    );
    DrawRectangleLines(
        static_cast<int>(borderX + borderSize - 12), 
        static_cast<int>(borderY + borderSize - 12), 
        static_cast<int>(cornerSize), 
        static_cast<int>(cornerSize), 
        accentColor
    );
}