#pragma once

#include <raylib.h>
#include <vector>
#include "chess_piece_graphic.h"
#include "chess_logic.h"

// This class is responsible for the graphical user interface of the chess program
class ChessGUI {

public:
    ChessGUI(ChessLogic& logic); // Constructor with logic reference
    ~ChessGUI(); // Destructor

    void draw(); // Draw GUI elements
    void update(); // Update GUI elements

    void initChessPieces(); // Initialize chess piece drawings based on logic
    void syncWithLogic(); // Sync GUI pieces with logic board state
    
private:
    Texture2D boardTexture; // Texture for the chess board
    std::vector<ChessPieceGraphic> piecesInPlay; // Vector to hold graphical chess pieces in play
    std::vector<ChessPieceGraphic> piecesCaptured; // Vector to hold graphical chess pieces captured
    ChessLogic& chessLogic; // Reference to the chess logic
    
    // Constants
    const float boardScale = 0.65f; // The scale to draw the board
    const float actualBoardSize = 1600.0f * boardScale; // Actual size of the board in px
    const float squareSize = actualBoardSize / 8.0f; // Size of the squares of the board
    const float pieceSize = squareSize * 0.8f; // Size of the pieces to draw
    const float pieceScale = pieceSize / 150.0f; // Scale of the pieces
    const float boardOffsetX = 0.0f;
    const float boardOffsetY = 0.0f;


    // Helper methods
    Vector2 boardToScreenPosition(int rank, int file) const;

};