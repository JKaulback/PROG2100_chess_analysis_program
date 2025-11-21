#pragma once

#include <raylib.h>
#include <utility>
#include <vector>

class ChessAnalysisProgram;
class TextureManager;

/**
 * Handles rendering captured pieces with styled backgrounds similar to the board border
 */
class CapturedPiecesRenderer {
public:
    CapturedPiecesRenderer(const ChessAnalysisProgram& controller, const TextureManager& textureManager);
    
    void draw() const;

private:
    const ChessAnalysisProgram& controller;
    const TextureManager& textureManager;
    
    void drawCapturedPieces() const;
    void drawCapturedPiecesBackground(float x, float y, float width, float height, bool isWhite) const;
    void drawCapturedPiecesLabels(float whiteX, float whiteY, float blackX, float blackY, float width, bool isFlipped) const;
    void drawBorderLayers(float borderX, float borderY, float borderWidth, float borderHeight) const;
    void drawBorderHighlights(float borderX, float borderY, float borderWidth, float borderHeight) const;
    void drawCornerDecorations(float borderX, float borderY, float borderWidth, float borderHeight) const;
    void drawSingleCornerDecoration(float x, float y, float cornerSize, const Color& decorColor, const Color& accentColor) const;
    
    std::pair<int, int> getCapturedPiecePosition(const int numPieces, const char pieceOwner) const;
    std::pair<float, float> getCapturedAreaDimensions(const std::vector<char>& capturedPieces, bool isWhite) const;
};