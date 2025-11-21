#pragma once

#include "../chess_move.h"
#include "../board/chess_board.h"
#include "../game_state/chess_game_state.h"

class BasicMoveValidator {
public:
    enum class ValidationResult {
        VALID,
        INVALID_OUT_OF_BOUNDS,
        INVALID_SAME_POSITION,
        INVALID_NO_PIECE,
        INVALID_WRONG_TURN,
        INVALID_DESTINATION
    };

    BasicMoveValidator() = default;
    ~BasicMoveValidator() = default;

    ValidationResult validateBasics(const ChessBoard& board, const ChessGameState& gameState, const ChessMove& move) const;

private:
    bool checkDestinationSquare(const ChessBoard& board, const ChessMove& move) const;
};
