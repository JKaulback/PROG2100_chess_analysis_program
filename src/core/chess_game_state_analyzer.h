#pragma once

#include "chess_logic.h"
#include "chess_move_validator.h"

class ChessGameStateAnalyzer {
public:
    enum class GameState {
        IN_PROGRESS,
        WHITE_WIN,
        BLACK_WIN,
        STALEMATE,
        DRAW_50_MOVES,
        DRAW_THREEFOLD_REPETITION,
        DRAW_INSUFFICIENT_MATERIAL
    };

    GameState analyzeGameState(const ChessLogic& logic);
private:
    // State changing checks
    bool isDraw50Moves(const ChessLogic& logic) const;
    bool isInsufficientMaterial(const ChessLogic& logic) const;
    bool isCheckmate(const ChessLogic& logic) const;
    bool isStalemate(const ChessLogic& logic) const;

    // Helper methods for state changes
    bool isInCheck(const ChessLogic& logic) const;
    bool hasLegalMoves(const ChessLogic& logic) const;
};