#pragma once

#include "board/chess_board.h"
#include "game_state/chess_game_state.h"
#include "chess_move_validator.h"
#include "chess_move.h"

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
    // Accesses the game state
    GameState analyzeGameState(const ChessBoard& board, const ChessGameState& gameState);
private:
    ChessMoveValidator validator;
    // State changing checks
    bool isDraw50Moves(const ChessGameState& gameState) const;
    bool isInsufficientMaterial(const ChessBoard& board) const;
    bool isCheckmate(const ChessBoard& board, const ChessGameState& gameState) const;
    bool isStalemate(const ChessBoard& board, const ChessGameState& gameState) const;
    bool isThreefoldRepetition(const ChessGameState& gameState) const;

    // Helper methods for state changes
    bool isInCheck(const ChessBoard& board, const ChessGameState& gameState) const;
    bool hasLegalMoves(const ChessBoard& board, const ChessGameState& gameState) const;
};