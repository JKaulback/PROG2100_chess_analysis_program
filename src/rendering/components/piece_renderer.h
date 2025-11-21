#pragma once

#include <raylib.h>
#include <utility>

class ChessAnalysisProgram;
class TextureManager;

/**
 * Handles rendering chess pieces on the board, including drag & drop and captured pieces
 */
class PieceRenderer {
public:
    PieceRenderer(const ChessAnalysisProgram& controller, const TextureManager& textureManager);
    
    void draw() const;
    
    // Board utilities
    Vector2 screenPosToBoardPos(const Vector2 pos) const;
    Vector2 boardPosToScreenPos(const Vector2 pos) const;

private:
    const ChessAnalysisProgram& controller;
    const TextureManager& textureManager;
    
    void drawPieces() const;
    void drawDraggedPiece() const;
    void drawCapturedPieces() const;
    
    std::pair<int, int> getCapturedPiecePosition(const int numPieces, const char pieceOwner) const;
};