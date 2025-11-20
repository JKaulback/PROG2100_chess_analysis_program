#pragma once

#include <raylib.h>
#include "../core/chess_move.h"

// Forward declarations to avoid circular dependency
class ChessGUI;
class ChessAnalysisProgram;

// This class handles all user input for the chess program
class ChessInputHandler {
public:
    ChessInputHandler(ChessAnalysisProgram& controller); // Constructor
    ~ChessInputHandler(); // Destructor

    void handleInput(const ChessGUI& gui); // Process input through controller

    // Getters for drag state (used by GUI for rendering)
    bool getIsDragging() const;
    int getDraggedPieceRank() const;
    int getDraggedPieceFile() const;
    Vector2 getDragOffset() const;
    char getDraggedPiece() const;

    // Toggle UCIEngine (start and stop engine)
    void toggleUCIEngine();
private:
    // Drag and drop state
    bool isDragging; // Whether a piece is being dragged
    int draggedPieceRank; // The original rank of a dragged piece
    int draggedPieceFile; // The original file of a dragged piece
    Vector2 dragOffset; // The offset from the mouse to prevent "jump"
    char draggedPiece; // The actual piece being dragged
    ChessAnalysisProgram& controller; // Reference to the controller
    
    // Helper methods
    void resetDragState(); // Reset drag state variables
};