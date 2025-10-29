#pragma once

#include <raylib.h>
#include "chess_logic.h"
#include "chess_gui.h"

// This class manages the overall chess analysis program
class ChessAnalysisProgram {

public:
    ChessAnalysisProgram(); // Constructor - now creates its own objects
    ~ChessAnalysisProgram(); // Destructor

    void run(); // Main loop

    // Controller methods - handle user interactions
    void handleInput(); // Process input and coordinate between GUI and Logic
    ChessLogic::Piece getPieceAt(int rank, int file) const; // Delegate to logic
    void makeMove(int fromRank, int fromFile, int toRank, int toFile); // Delegate to logic
    std::string pieceToString(ChessLogic::Piece piece) const; // Delegate to logic
    
    // Methods for GUI to access drag state
    bool getIsDragging() const { return isDragging; }
    int getDraggedPieceRank() const { return draggedPieceRank; }
    int getDraggedPieceFile() const { return draggedPieceFile; }
    Vector2 getDragOffset() const { return dragOffset; }
    ChessLogic::Piece getDraggedPiece() const { return draggedPiece; }

private:
    ChessLogic logic; // Own the logic object
    ChessGUI gui; // Own the GUI object
    
    // Drag and drop state (moved from GUI)
    bool isDragging; // Whether a piece is being dragged
    int draggedPieceRank; // The original rank of a dragged piece
    int draggedPieceFile; // The original file of a dragged piece
    Vector2 dragOffset; // The offset from the mouse to prevent "jump"
    ChessLogic::Piece draggedPiece; // The actual piece being dragged
    
    // Helper methods
    void resetDragState(); // Reset drag state variables
};