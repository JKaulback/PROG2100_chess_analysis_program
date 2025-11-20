#pragma once

#include <raylib.h>
#include <vector>
#include "../analysis_engine/uci_engine.h"
#include "../core/chess_move_validator.h"
#include "../core/chess_move.h"
#include "../core/board/chess_board.h"
#include "../core/game_state/chess_game_state.h"
#include "../core/game_state/chess_game_state_analyzer.h"
#include "../core/game_state/fen_position_tracker.h"
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
    char getPieceAt(const int rank, const int file) const; // Delegate to logic
    std::string pieceToTextureString(const char piece) const; // Delegate to logic
    char getCurrentPlayer() const; // Delegate to logic for turn info
    std::vector<char> getCapturedPieces() const; // Delegate to logic
    char getPieceOwner(const int rank, const int file) const; // Delegate to logic
    char getPieceOwner(const char piece) const; // Delegate to logic
    int getHalfmoveClock() const; // Delegate to logic
    std::string getCurrentFENString() const; // Delegate to logic

    // Delegate methods to access input handler (for GUI)
    bool getIsDragging() const; // Delegate to input handler
    int getDraggedPieceRank() const; // Delegate to input handler
    int getDraggedPieceFile() const; // Delegate to input handler
    Vector2 getDragOffset() const; // Delegate to input handler
    char getDraggedPiece() const; // Delegate to input handler

    // Move validation and execution methods (Controller orchestration)
    bool attemptMove(const ChessMove& move); // Validate and execute move
    
    // Game state access for GUI drawing
    GameState getGameState() const;
    bool isGameOver() const;
    std::string getGameOverString() const; // Get a game over string mapped to a game state
    
    // FEN loader support methods
    void setPieceAt(const int rank, const int file, const char piece);
    void clearBoard();
    void setCurrentPlayer(const char player);
    void setCastlingRights(bool whiteKingside, bool whiteQueenside, bool blackKingside, bool blackQueenside);
    void setEnPassantTarget(const int rank, const int file);
    void clearEnPassantTarget();
    void setHalfmoveClock(const int halfmoves);
    void setFullmoveClock(const int fullmoves);

    // UCI Engine state access and processing
    void enableUCIEngine();
    void disableUCIEngine();
    void toggleUCIEngine();
    bool isUCIEngineEnabled() const;
    void setUCIEnginePosition();
    EngineAnalysis pollUCIEngineAnalysis();
private:
    // Helper methods
    bool isValidMoveResult(MoveResult result) const; // Check if move result indicates success
    void setUCIEngineStateInGUI(const bool isEnabled);

    // Game State Management
    ChessBoard board;
    ChessGameState gameState;    
    FENPositionTracker fenStateHistory;

    // Validation
    ChessMoveValidator moveValidator; // Own the move validator object
    
    // Analysis & UI
    ChessGUI gui; // Own the GUI object
    ChessInputHandler inputHandler; // Own the input handler object
    ChessGameStateAnalyzer gameStateAnalyzer; // Own the game state analyzer object
    std::unique_ptr<UCIEngine> uciEngine; // Own the Stockfish move analysis engine manager object

    // Current state
    GameState currentGameState;
};