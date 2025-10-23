#pragma once

#include <raylib.h>
#include <map>
#include "chess_logic.h"

// This class is responsible for the graphical user interface of the chess program
class ChessGUI {

public:
    ChessGUI(ChessLogic& logic); // Constructor with logic reference
    ~ChessGUI(); // Destructor

    void draw(); // Draw GUI elements
    void update(); // Update GUI elements
    
private:
    // Chess board and piece display
    Texture2D boardTexture; // Texture for the chess board
    std::map<std::string, Texture2D> pieceTextures;
    ChessLogic& chessLogic; // Reference to the chess logic
    
    // Board texture data
    const float boardScale = 0.65f; // The scale to draw the board
    const float actualBoardSize = 1600.0f * boardScale; // Actual size of the board in px
    
    const float squareSize = actualBoardSize / 8.0f; // Size of the squares of the board
    const float boardOffsetX = 0.0f;
    const float boardOffsetY = 0.0f;
    const int minRank = 0;
    const int minFile = 0;
    const int maxRank = 7;
    const int maxFile = 7;

    // Piece texture data
    const float pieceSize = squareSize * 0.8f; // Size of the pieces to draw
    const float pieceScale = pieceSize / 150.0f; // Scale of the pieces
    const float centeredValue = (squareSize / 2.0f) - (pieceSize / 2.0f); // Value to add to piece position for centering
    
    // Drag and drop state
    bool isDragging; // Whether a piece is being dragged
    int draggedPieceRank; // The original rank of a dragged piece
    int draggedPieceFile; // The original file of a dragged piece
    Vector2 dragOffset; // The offset from the mouse to prevent "jump"
    ChessPiece draggedPiece; // The actual piece being dragged

    // Helper methods
    Vector2 screenPosToBoardPos(Vector2) const; // Translate screen position to board position
    Vector2 boardPosToScreenPos(Vector2) const; // Translate board position to screen position
    void initPieceTextures(); // Initialize all piece textures
    void loadPieceTexture(const std::string); // Load a single piece texture
    void drawChessPieces(); // Draw the chess pieces based on their logical position
    
};