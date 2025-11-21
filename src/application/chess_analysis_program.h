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

class FENLoader;

// This class manages the overall chess analysis program
class ChessAnalysisProgram {
public:
    ChessAnalysisProgram(); // Constructor - now creates its own objects
    ~ChessAnalysisProgram(); // Destructor

    void run(); // Main loop

    // Delegate methods to access logic (for GUI)
    char getPieceAt(const int rank, const int file) const { return board.getPieceAt(rank, file); }
    std::string pieceToTextureString(const char piece) const { return board.getPieceTextureString(piece); }
    char getCurrentPlayer() const { return gameState.getCurrentPlayer(); }
    std::vector<char> getCapturedPieces() const { return board.getCapturedPieces(); }
    std::vector<char> getWhiteCapturedPieces() const { return board.getWhiteCapturedPieces(); }
    std::vector<char> getBlackCapturedPieces() const { return board.getBlackCapturedPieces(); }
    char getPieceOwner(const int rank, const int file) const { return board.getPieceOwner(rank, file); }
    char getPieceOwner(const char piece) const { return board.getPieceOwner(piece); }
    int getHalfmoveClock() const { return gameState.getHalfmoveClock(); }
    std::string getCurrentFENString() const { return fenStateHistory.getCurrentPosition(); }

    // Delegate methods to access input handler (for GUI)
    bool getIsDragging() const { return inputHandler.getIsDragging(); }
    int getDraggedPieceRank() const { return inputHandler.getDraggedPieceRank(); }
    int getDraggedPieceFile() const { return inputHandler.getDraggedPieceFile(); }
    Vector2 getDragOffset() const { return inputHandler.getDragOffset(); }
    char getDraggedPiece() const { return inputHandler.getDraggedPiece(); }

    // Move validation and execution methods (Controller orchestration)
    bool attemptMove(const ChessMove& move); // Validate and execute move
    
    // Game state access for GUI drawing
    GameState getGameState() const { return currentGameState; }
    bool isGameOver() const { return currentGameState != GameState::IN_PROGRESS; }
    std::string getGameOverString() const; // Get a game over string mapped to a game state
    
    // FEN position tracking support methods
    void undoMove();
    void redoMove();
    const std::vector<PositionState>& getPositionHistory() const { return fenStateHistory.getPositionHistory(); }
    const std::vector<PositionState>& getRedoPositions() const {return fenStateHistory.getRedoPositions(); }

    // FEN loader support methods
    void setPieceAt(const int rank, const int file, const char piece) { board.setPieceAt(rank, file, piece); }
    void clearBoard();
    void clearBoardOnly(); // Clear board without affecting history
    void applyFenDirect(const std::string& fenString); // Direct FEN application for undo/redo
    void applyPositionState(const PositionState& state); // Apply complete position state including captured pieces
    void setCurrentPlayer(const char player) { gameState.setCurrentPlayer(player); }
    void setCastlingRights(bool whiteKingside, bool whiteQueenside, bool blackKingside, bool blackQueenside) {
        gameState.setCastlingRights(whiteKingside, whiteQueenside, blackKingside, blackQueenside);
    }
    void setEnPassantTarget(const int rank, const int file) { gameState.setEnPassantTarget(rank, file); }
    void clearEnPassantTarget() { gameState.clearEnPassantState(); }
    void setHalfmoveClock(const int halfmoves) { gameState.setHalfmoveClock(halfmoves); }
    void setFullmoveClock(const int fullmoves) { gameState.setFullmoveClock(fullmoves); }
    void applyFen(const std::string& fenString);

    // UCI Engine state access and processing
    void enableUCIEngine();
    void disableUCIEngine();
    void toggleUCIEngine();
    bool isUCIEngineEnabled() const { return uciEngine->isEnabled(); }
    void setUCIEnginePosition();
    EngineAnalysis pollUCIEngineAnalysis() const { return uciEngine->pollAnalysis(); }
    
    // Game reset functionality
    void resetToInitialPosition();
    
    // Board display options
    void toggleBoardFlip() { isBoardFlipped = !isBoardFlipped; }
    bool getBoardFlipped() const { return isBoardFlipped; }
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
    std::unique_ptr<ChessGUI> gui; // Own the GUI object
    ChessInputHandler inputHandler; // Own the input handler object
    ChessGameStateAnalyzer gameStateAnalyzer; // Own the game state analyzer object
    std::unique_ptr<UCIEngine> uciEngine; // Own the Stockfish move analysis engine manager object

    // Current state
    GameState currentGameState;
    
    // Display options
    bool isBoardFlipped = false;
};