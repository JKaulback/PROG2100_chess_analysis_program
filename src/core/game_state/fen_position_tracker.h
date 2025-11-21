#pragma once

#include <sstream>
#include <string>
#include <vector>
#include "../board/chess_board.h"
#include "chess_game_state.h"

// Structure to hold complete position state including captured pieces
struct PositionState {
    std::string fenString;
    std::vector<char> whiteCapturedPieces;
    std::vector<char> blackCapturedPieces;
    std::string algebraicMove; // The move that led to this position (empty for initial position)
    bool isActive = true;

    PositionState() = default;
    PositionState(const std::string& fen, const std::vector<char>& whiteCaptured, const std::vector<char>& blackCaptured, const std::string& move = "")
        : fenString(fen), whiteCapturedPieces(whiteCaptured), blackCapturedPieces(blackCaptured), algebraicMove(move) {}
};

class FENPositionTracker {
public:
    FENPositionTracker();
    
    // Position tracking
   const std::vector<PositionState>& getPositionHistory() const;
    std::string getStartPosition() const;
    std::string getCurrentPosition() const;

    // Record once
    void record(const ChessBoard& board, const ChessGameState& gameState, const std::string& algebraicMove);
    void record(const ChessBoard& board, const ChessGameState& gameState);

    // Manage undo/redo state
    void undoMove();
    void redoMove();
    const bool isUndoAvailable() const;
    const bool isRedoAvailable() const;
    const std::string getRedoPosition() const;
    const std::string getRedoMove() const;
    const std::vector<PositionState>& getRedoPositions() const;
    
    // Get complete position state (FEN + captured pieces + move)
    PositionState getCurrentPositionState() const;
    PositionState getRedoPositionState() const;
    
    // Get move that led to current position
    std::string getCurrentMove() const;

    // For UCI support
    void setStartingPosition(const std::string& fen);
    void clearHistory();
    const std::vector<std::string> getMoveHistory() const;

    // Game state accessors
    bool isThreefoldRepetition() const;

private:
    std::vector<PositionState> positionHistory;
    std::vector<PositionState> positionRedo;

    // Position tracking helpers
    std::string getBoardState(const ChessBoard& board) const;
    std::string getCastlingRights(const ChessGameState& gameState) const;
    std::string getEnPassantTarget(const ChessGameState& gameState) const;

    // FEN string data extraction
    std::string extractPositionKey(const std::string& fenString) const;
};