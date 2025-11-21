#pragma once

#include <string>
#include <map>
#include "../config/config.h"
#include "board/chess_board.h"
#include "game_state/chess_game_state.h"
#include "chess_move.h"
#include "validators/basic_move_validator.h"
#include "validators/piece_movement_validator.h"
#include "validators/special_move_validator.h"
#include "validators/check_validator.h"

class ChessMoveValidator {
public:
    enum class MoveResult {
        VALID,
        INVALID_OUT_OF_BOUNDS,
        INVALID_SAME_POSITION,
        INVALID_NO_PIECE,
        INVALID_WRONG_TURN,
        INVALID_ILLEGAL_MOVE,
        
        // Special move results
        VALID_CASTLE_KINGSIDE,
        VALID_CASTLE_QUEENSIDE,
        VALID_EN_PASSANT,
        VALID_PROMOTION,
    };

    ChessMoveValidator() = default;
    ~ChessMoveValidator() = default;

    // Main validation function
    MoveResult validateMove(const ChessBoard& board, const ChessGameState& gameState, const ChessMove& move) const;

    // Helper functions for result validation
    bool isValidMoveResult(const MoveResult result) const;
    bool isInvalidMoveResult(const MoveResult result) const;

    // Delegate methods for backward compatibility (if needed)
    bool isSquareUnderAttack(const ChessBoard& board, const ChessGameState& gameState, const int defRank, const int defFile, const char attackingPlayer) const;
    bool wouldLeaveKingInCheck(const ChessBoard& board, const ChessGameState& gameState, const ChessMove& move) const;

private:
    // Specialized validators
    BasicMoveValidator basicValidator;
    PieceMovementValidator pieceValidator;
    SpecialMoveValidator specialValidator;
    CheckValidator checkValidator;
    
    // Helper method to convert BasicMoveValidator result to MoveResult
    MoveResult convertBasicResult(BasicMoveValidator::ValidationResult result) const;
};