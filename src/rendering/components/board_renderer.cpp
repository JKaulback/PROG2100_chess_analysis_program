#include "board_renderer.h"
#include "texture_manager.h"
#include "../../config/config.h"

namespace BoardCfg = Config::Board;
namespace DecorCfg = Config::Decorations;
namespace BorderCfg = Config::Borders;

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
    const float borderWidth = BorderCfg::BOARD_BORDER_WIDTH;
    
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
    const float shadowOffset = BorderCfg::SHADOW_OFFSET_BOARD;
    const Color borderColor = BorderCfg::BORDER_COLOR;
    const Color innerBorder = BorderCfg::INNER_BORDER;
    const Color shadowColor = BorderCfg::SHADOW_COLOR;
    const float accentWidth = BorderCfg::ACCENT_WIDTH_BOARD;
    
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
    const Color highlightColor = BorderCfg::HIGHLIGHT_COLOR;
    const float accentWidth = BorderCfg::ACCENT_WIDTH_BOARD;
    const float highlightWidth = BorderCfg::HIGHLIGHT_WIDTH;
    
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
    const float cornerSize = DecorCfg::CORNER_SIZE_BOARD;
    const float cornerOffset = DecorCfg::CORNER_OFFSET_BOARD;
    const float cornerInset = DecorCfg::CORNER_INSET_BOARD;
    const Color decorColor = DecorCfg::DECOR_COLOR;
    const Color accentColor = DecorCfg::ACCENT_COLOR;
    
    // Draw all four corners using the helper method
    drawSingleCornerDecoration(borderX - cornerOffset, borderY - cornerOffset, cornerSize, decorColor, accentColor);  // Top-left
    drawSingleCornerDecoration(borderX + borderSize - cornerInset, borderY - cornerOffset, cornerSize, decorColor, accentColor);  // Top-right
    drawSingleCornerDecoration(borderX - cornerOffset, borderY + borderSize - cornerInset, cornerSize, decorColor, accentColor);  // Bottom-left
    drawSingleCornerDecoration(borderX + borderSize - cornerInset, borderY + borderSize - cornerInset, cornerSize, decorColor, accentColor);  // Bottom-right
}