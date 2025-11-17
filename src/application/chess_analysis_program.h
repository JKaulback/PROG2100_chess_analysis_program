#pragma once

#include <raylib.h>
#include <vector>
#include "../core/chess_game_state_analyzer.h"
#include "../core/chess_logic.h"
#include "../core/chess_move_validator.h"
#include "../core/chess_move.h"
#include "../rendering/chess_gui.h"
#include "../input/chess_input_handler.h"

// Type alias for cleaner code
using MoveResult = ChessMoveValidator::MoveResult;
using GameState = ChessGameStateAnalyzer::GameState;

// This class manages the overall chess analysis program
class ChessAnalysisProgram {
public:
    ChessAnalysisProgram(); // Constructor - now creates its own objects
    ~ChessAnalysisProgram(); // Destructor

    void run(); // Main loop

    // Delegate methods to access logic (for GUI)
    ChessLogic::Piece getPieceAt(const int rank, const int file) const; // Delegate to logic
    std::string pieceToTextureString(const ChessLogic::Piece piece) const; // Delegate to logic
    ChessLogic::Player getCurrentPlayer() const; // Delegate to logic for turn info
    std::vector<ChessLogic::Piece> getCapturedPieces() const; // Delegate to logic
    ChessLogic::Player getPieceOwner(const ChessLogic::Piece piece) const; // Delegate to logic
    int getHalfmoveClock() const; // Delegate to logic
    std::string getCurrentFENString() const; // Delegate to logic

    // Delegate methods to access input handler (for GUI)
    bool getIsDragging() const; // Delegate to input handler
    int getDraggedPieceRank() const; // Delegate to input handler
    int getDraggedPieceFile() const; // Delegate to input handler
    Vector2 getDragOffset() const; // Delegate to input handler
    ChessLogic::Piece getDraggedPiece() const; // Delegate to input handler

    // Move validation and execution methods (Controller orchestration)
    bool attemptMove(const ChessMove& move); // Validate and execute move
    
    // Game state access for GUI drawing
    GameState getGameState() const;
    bool isGameOver() const;
    std::string getGameOverString() const; // Get a game over string mapped to a game state
private:
    // Helper methods
    bool isValidMoveResult(MoveResult result) const; // Check if move result indicates success
private:
    ChessLogic logic; // Own the logic object
    ChessGUI gui; // Own the GUI object
    ChessInputHandler inputHandler; // Own the input handler object
    ChessMoveValidator moveValidator; // Own the move validator object
    ChessGameStateAnalyzer gameStateAnalyzer; // Own the game state analyzer object
    GameState currentGameState;
};