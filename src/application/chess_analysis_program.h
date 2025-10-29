#pragma once

#include <raylib.h>
#include "../core/chess_logic.h"
#include "../rendering/chess_gui.h"
#include "../input/chess_input_handler.h"

// This class manages the overall chess analysis program
class ChessAnalysisProgram {

public:
    ChessAnalysisProgram(); // Constructor - now creates its own objects
    ~ChessAnalysisProgram(); // Destructor

    void run(); // Main loop

    // Delegate methods to access logic (for GUI)
    ChessLogic::Piece getPieceAt(int rank, int file) const; // Delegate to logic
    std::string pieceToString(ChessLogic::Piece piece) const; // Delegate to logic
    
    // Delegate methods to access input handler (for GUI)
    bool getIsDragging() const; // Delegate to input handler
    int getDraggedPieceRank() const; // Delegate to input handler
    int getDraggedPieceFile() const; // Delegate to input handler
    Vector2 getDragOffset() const; // Delegate to input handler
    ChessLogic::Piece getDraggedPiece() const; // Delegate to input handler

private:
    ChessLogic logic; // Own the logic object
    ChessGUI gui; // Own the GUI object
    ChessInputHandler inputHandler; // Own the input handler object
};