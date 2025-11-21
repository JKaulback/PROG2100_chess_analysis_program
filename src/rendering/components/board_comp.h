#pragma once

#include <raylib.h>
#include <map>
#include <string>
#include "../../application/chess_analysis_program.h"

class ChessAnalysisProgram;

class BoardComp {
public:
    BoardComp(const ChessAnalysisProgram& controller);
    ~BoardComp();
    
    void draw() const;
    
    // Board utilities
    Vector2 screenPosToBoardPos(const Vector2 pos) const;
    Vector2 boardPosToScreenPos(const Vector2 pos) const;
    float getSquareSize() const;
    float getPieceSize() const;

private:
    const ChessAnalysisProgram& controller;
    Texture2D boardTexture;
    std::map<std::string, Texture2D> pieceTextures;
    bool texturesLoaded;
    
    void initTextures();
    void initPieceTexture(const char piece);
    void drawBoard() const;
    void drawCoordinates() const;
    void drawPieces() const;
    void drawDraggedPiece() const;
    void drawCapturedPieces() const;

    std::pair<int, int> getCapturedPiecePosition(const int numPieces, const char pieceOwner) const;
};