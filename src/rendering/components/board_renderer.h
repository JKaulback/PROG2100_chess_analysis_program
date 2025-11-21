#pragma once

#include <raylib.h>

class TextureManager;

/**
 * Handles rendering the chess board background, borders, and decorative elements
 */
class BoardRenderer {
public:
    BoardRenderer(const TextureManager& textureManager);
    
    void draw() const;

private:
    const TextureManager& textureManager;
    
    void drawBoard() const;
    void drawBoardBorder() const;
    void drawCornerDecorations(float borderX, float borderY, float borderSize) const;
};