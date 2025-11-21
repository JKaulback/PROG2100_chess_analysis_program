#pragma once

#include "../chess_move.h"
#include "../board/chess_board.h"
#include "../game_state/chess_game_state.h"

class PieceMovementValidator {
public:
    PieceMovementValidator() = default;
    ~PieceMovementValidator() = default;

    bool validatePieceMovement(const ChessBoard& board, const ChessGameState& gameState, const ChessMove& move) const;
    bool validateBasicPieceMovement(const ChessBoard& board, const ChessGameState& gameState, const char piece, const ChessMove& move) const;
    
    // Piece specific validation (made public for use by SpecialMoveValidator)
    bool validatePawnMove(const ChessBoard& board, const ChessGameState& gameState, const ChessMove& move) const;

private:
    // Helper function for sliding pieces (rook, bishop, queen)
    bool isPathClearForSlidingPiece(const ChessBoard& board, const ChessMove& move) const;
};
