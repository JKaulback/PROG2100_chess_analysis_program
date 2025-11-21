#pragma once

#include <raylib.h>
#include <memory>
#include "texture_manager.h"
#include "board_renderer.h"
#include "coordinate_renderer.h"
#include "piece_renderer.h"

class ChessAnalysisProgram;

/**
 * Main board component that orchestrates all board rendering sub-components
 */
class BoardComp {
public:
    BoardComp(const ChessAnalysisProgram& controller);
    ~BoardComp() = default;
    
    void draw() const;
    
    // Board utilities
    Vector2 screenPosToBoardPos(const Vector2 pos) const;
    Vector2 boardPosToScreenPos(const Vector2 pos) const;
    float getSquareSize() const;
    float getPieceSize() const;

private:
    const ChessAnalysisProgram& controller;
    
    // Rendering components
    std::unique_ptr<TextureManager> textureManager;
    std::unique_ptr<BoardRenderer> boardRenderer;
    std::unique_ptr<CoordinateRenderer> coordinateRenderer;
    std::unique_ptr<PieceRenderer> pieceRenderer;
};