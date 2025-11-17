#pragma once

#include <string>
#include <map>
#include "../config/config.h"
#include "board/chess_board.h"
#include "game_state/chess_game_state.h"
#include "chess_move.h"

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

    // Main validation function - currently only checks board boundaries
    MoveResult validateMove(const ChessBoard& board, const ChessGameState& gameState, const ChessMove& move) const;

    // Helper function for move and state validation
    bool isSquareUnderAttack(const ChessBoard& board, const ChessGameState& gameState, const int defRank, const int defFile, const char attackingPlayer) const;
    bool wouldLeaveKingInCheck(const ChessBoard& board, const ChessGameState& gameState ,const ChessMove& move) const;
    bool isValidMoveResult(const MoveResult result) const;
    bool isInvalidMoveResult(const MoveResult result) const;
private:    
    // Chess rule validation
    bool validatePieceMovement(const ChessBoard& board, const ChessGameState& gameState, const ChessMove& move) const;
    bool checkDestinationSquare(const ChessBoard& board, const ChessMove& move) const;
    
    // Helper function for sliding pieces (rook, bishop, queen)
    bool isPathClearForSlidingPiece(const ChessBoard& board, const ChessMove& move) const;
    
    // Helper functions for check detection
    bool validateBasicPieceMovement(const ChessBoard& board, const ChessGameState& gameState, const char piece, const ChessMove& move) const;

    // Piece specific validation (only pawn needs special handling)
    bool validatePawnMove(const ChessBoard& board, const ChessGameState& gameState, const ChessMove& move) const;

    // Castling validation methods
    bool validateCastling(const ChessBoard& board, const ChessGameState& gameState, const ChessMove& move) const;
    bool canCastle(const ChessBoard& board, const ChessGameState& gameState, const char player, const bool isKingside) const;
    
    // En passant validation method
    bool validateEnPassant(const ChessBoard& board, const ChessGameState& gameState, const ChessMove& move) const;

    // Promotion validation method
    bool validatePromotion(const ChessBoard& board, const ChessGameState& gameState, const ChessMove& move) const;
};