#pragma once

#include <string>
#include <map>
#include "../config/config.h"
#include "chess_logic.h"

class ChessMoveValidator {
public:
    // Simple validation result enum - start with basic cases
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
        VALID_EN_PASSANT
    };

    ChessMoveValidator() = default;
    ~ChessMoveValidator() = default;

    // Main validation function - currently only checks board boundaries
    MoveResult validateMove(
        const ChessLogic& logic,
        int fromRank, 
        int fromFile, 
        int toRank, 
        int toFile
    ) const;
private:

    
    // Chess rule validation
    bool validatePieceMovement(const ChessLogic& logic, int fromRank, int fromFile, int toRank, int toFile) const;
    bool checkDestinationSquare(const ChessLogic& logic, int fromRank, int fromFile, int toRank, int toFile) const;
    bool wouldLeaveKingInCheck(const ChessLogic& logic, int fromRank, int fromFile, int toRank, int toFile) const;

    // Helper function for sliding pieces (rook, bishop, queen)
    bool isPathClearForSlidingPiece(const ChessLogic& logic, int fromRank, int fromFile, int toRank, int toFile) const;
    
    // Helper functions for check detection
    bool isSquareUnderAttack(const ChessLogic& logic, int rank, int file, ChessLogic::Player attackingPlayer) const;
    bool validateBasicPieceMovement(const ChessLogic& logic, ChessLogic::Piece piece, int fromRank, int fromFile, int toRank, int toFile) const;

    // Piece specific validation (only pawn needs special handling)
    bool validatePawnMove(const ChessLogic& logic, int fromRank, int fromFile, int toRank, int toFile) const;

    // Castling validation methods
    bool validateCastling(const ChessLogic& logic, int fromRank, int fromFile, int toRank, int toFile) const;
    bool canCastle(const ChessLogic& logic, ChessLogic::Player player, bool isKingside) const;
    
    // En passant validation method
    bool validateEnPassant(const ChessLogic& logic, int fromRank, int fromFile, int toRank, int toFile) const;

};