#pragma once

#include "../chess_move.h"
#include "../board/chess_board.h"
#include "../game_state/chess_game_state.h"

class SpecialMoveValidator {
public:
    SpecialMoveValidator() = default;
    ~SpecialMoveValidator() = default;

    // Castling validation methods
    bool validateCastling(const ChessBoard& board, const ChessGameState& gameState, const ChessMove& move) const;
    bool canCastle(const ChessBoard& board, const ChessGameState& gameState, const char player, const bool isKingside) const;
    
    // En passant validation method
    bool validateEnPassant(const ChessBoard& board, const ChessGameState& gameState, const ChessMove& move) const;

    // Promotion validation method
    bool validatePromotion(const ChessBoard& board, const ChessGameState& gameState, const ChessMove& move) const;
};
