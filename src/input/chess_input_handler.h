#pragma once

#include <raylib.h>
#include "../core/chess_logic.h"

// Forward declaration to avoid circular dependency
class ChessGUI;

// This class handles all user input for the chess program
class ChessInputHandler {

public:
    ChessInputHandler(); // Constructor
    ~ChessInputHandler(); // Destructor

    void handleInput(ChessLogic& logic, const ChessGUI& gui); // Process input and update game state

    // Getters for drag state (used by GUI for rendering)
    bool getIsDragging() const { return isDragging; }
    int getDraggedPieceRank() const { return draggedPieceRank; }
    int getDraggedPieceFile() const { return draggedPieceFile; }
    Vector2 getDragOffset() const { return dragOffset; }
    ChessLogic::Piece getDraggedPiece() const { return draggedPiece; }

private:
    // Drag and drop state
    bool isDragging; // Whether a piece is being dragged
    int draggedPieceRank; // The original rank of a dragged piece
    int draggedPieceFile; // The original file of a dragged piece
    Vector2 dragOffset; // The offset from the mouse to prevent "jump"
    ChessLogic::Piece draggedPiece; // The actual piece being dragged
    
    // Helper methods
    void resetDragState(); // Reset drag state variables
};