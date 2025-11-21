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
    
    PositionState() = default;
    PositionState(const std::string& fen, const std::vector<char>& whiteCaptured, const std::vector<char>& blackCaptured)
        : fenString(fen), whiteCapturedPieces(whiteCaptured), blackCapturedPieces(blackCaptured) {}
};

class FENPositionTracker {
public:
    FENPositionTracker();
    
    // Position tracking
    void recordPosition(const ChessBoard& board, const ChessGameState& gameState);
    const std::vector<std::string>& getPositionHistory() const;
    std::string getStartPosition() const;
    std::string getCurrentPosition() const;

    // Record once
    void record(const ChessBoard& board, const ChessGameState& gameState, const std::string& algebraicMove);

    // Manage undo/redo state
    void undoMove();
    void redoMove();
    const bool isUndoAvailable() const;
    const bool isRedoAvailable() const;
    const std::string getRedoPosition() const;
    const std::string getRedoMove() const;
    const std::vector<std::string>& getRedoPositions() const;
    const std::vector<std::string>& getRedoMoves() const;
    
    // Get complete position state (FEN + captured pieces)
    PositionState getCurrentPositionState() const;
    PositionState getRedoPositionState() const;

    // For UCI support
    void recordMove(const std::string& algebraicMove);
    void setStartingPosition(const std::string& fen);
    void clearHistory();
    const std::vector<std::string>& getMoveHistory() const;

    // Game state accessors
    bool isThreefoldRepetition() const;

private:
    std::vector<PositionState> positionHistory;
    std::vector<PositionState> positionRedo;
    std::vector<std::string> moves;
    std::vector<std::string> movesRedo;

    // Position tracking helpers
    std::string getBoardState(const ChessBoard& board) const;
    std::string getCastlingRights(const ChessGameState& gameState) const;
    std::string getEnPassantTarget(const ChessGameState& gameState) const;

    // FEN string data extraction
    std::string extractPositionKey(const std::string& fenString) const;
};