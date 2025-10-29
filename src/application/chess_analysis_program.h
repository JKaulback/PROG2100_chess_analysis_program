#pragma once

#include <raylib.h>
#include "../core/chess_logic.h"
#include "../rendering/chess_gui.h"
#include "../input/chess_input_handler.h"
#include "../core/chess_move_validator.h"

// Type alias for cleaner code
using MoveResult = ChessMoveValidator::MoveResult;

// This class manages the overall chess analysis program
class ChessAnalysisProgram {
public:
    ChessAnalysisProgram(); // Constructor - now creates its own objects
    ~ChessAnalysisProgram(); // Destructor

    void run(); // Main loop

    // Delegate methods to access logic (for GUI)
    ChessLogic::Piece getPieceAt(int rank, int file) const; // Delegate to logic
    std::string pieceToString(ChessLogic::Piece piece) const; // Delegate to logic
    ChessLogic::Player getCurrentPlayer() const; // Delegate to logic for turn info
    
    // Delegate methods to access input handler (for GUI)
    bool getIsDragging() const; // Delegate to input handler
    int getDraggedPieceRank() const; // Delegate to input handler
    int getDraggedPieceFile() const; // Delegate to input handler
    Vector2 getDragOffset() const; // Delegate to input handler
    ChessLogic::Piece getDraggedPiece() const; // Delegate to input handler
    
    // Move validation and execution methods (Controller orchestration)
    bool attemptMove(int srcRank, int srcFile, int destRank, int destFile); // Validate and execute move
    MoveResult validateMove(int srcRank, int srcFile, int destRank, int destFile) const; // Just validate
    std::string getMoveValidationMessage(int srcRank, int srcFile, int destRank, int destFile) const; // Get validation message
private:
    ChessLogic logic; // Own the logic object
    ChessGUI gui; // Own the GUI object
    ChessInputHandler inputHandler; // Own the input handler object
    ChessMoveValidator moveValidator; // Own the move validator object
};