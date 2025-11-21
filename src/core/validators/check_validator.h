#pragma once

#include "../chess_move.h"
#include "../board/chess_board.h"
#include "../game_state/chess_game_state.h"

class CheckValidator {
public:
    CheckValidator() = default;
    ~CheckValidator() = default;

    // Helper function for move and state validation
    bool isSquareUnderAttack(const ChessBoard& board, const ChessGameState& gameState, const int defRank, const int defFile, const char attackingPlayer) const;
    bool wouldLeaveKingInCheck(const ChessBoard& board, const ChessGameState& gameState, const ChessMove& move) const;
};
