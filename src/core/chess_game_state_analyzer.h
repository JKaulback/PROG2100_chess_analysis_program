#pragma once

#include "chess_logic.h"

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
    bool isDraw50Moves(const ChessLogic& logic);
    bool isInsufficientMaterial(const ChessLogic& logic);
};