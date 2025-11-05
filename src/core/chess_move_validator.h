#pragma once

#include <string>
#include <map>
#include "../config/config.h"
#include "chess_logic.h"

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
    MoveResult validateMove(const ChessLogic& logic, int srcRank, int srcFile, int destRank, int destFile) const;

    // Helper function for move and state validation
    bool isSquareUnderAttack(const ChessLogic& logic, const int defRank, const int defFile, const ChessLogic::Player attackingPlayer) const;
    bool wouldLeaveKingInCheck(const ChessLogic& logic, const int srcRank, const int srcFile, const int destRank, const int destFile) const;
    bool isValidMoveResult(const MoveResult result) const;
    bool isInvalidMoveResult(const MoveResult result) const;
private:    
    // Chess rule validation
    bool validatePieceMovement(const ChessLogic& logic, const int srcRank, const int srcFile, const int destRank, const int destFile) const;
    bool checkDestinationSquare(const ChessLogic& logic, const int srcRank, const int srcFile, const int destRank, const int destFile) const;
    
    // Helper function for sliding pieces (rook, bishop, queen)
    bool isPathClearForSlidingPiece(const ChessLogic& logic, const int srcRank, const int srcFile, const int destRank, const int destFile) const;
    
    // Helper functions for check detection
    bool validateBasicPieceMovement(const ChessLogic& logic, const ChessLogic::Piece piece, const int srcRank, const int srcFile, const int destRank, const int destFile) const;

    // Piece specific validation (only pawn needs special handling)
    bool validatePawnMove(const ChessLogic& logic, const int srcRank, const int srcFile, const int destRank, const int destFile) const;

    // Castling validation methods
    bool validateCastling(const ChessLogic& logic, const int srcRank, const int srcFile, const int destRank, const int destFile) const;
    bool canCastle(const ChessLogic& logic, const ChessLogic::Player player, const bool isKingside) const;
    
    // En passant validation method
    bool validateEnPassant(const ChessLogic& logic, const int srcRank, const int srcFile, const int destRank, const int destFile) const;

    // Promotion validation method
    bool validatePromotion(const ChessLogic& logic, const int srcRank, const int srcFile, const int destRank, const int destFile) const;
};