#pragma once

#include <sstream>
#include <string>
#include <vector>
#include "../board/chess_board.h"
#include "chess_game_state.h"

class FENPositionTracker {
public:
    FENPositionTracker();
    
    // Position tracking
    void recordPosition(const ChessBoard& board, const ChessGameState& gameState);
    std::vector<std::string> getPositionHistory() const;
    
    // Game state accessors
    bool isThreefoldRepetition() const;

private:
    std::vector<std::string> positionHistory;

    // Position tracking helpers
    std::string getBoardState(const ChessBoard& board) const;
    std::string getCastlingRights(const ChessGameState& gameState) const;
    std::string getEnPassantTarget(const ChessGameState& gameState) const;

    // FEN string data extraction
    std::string extractPositionKey(const std::string& fenString) const;
};