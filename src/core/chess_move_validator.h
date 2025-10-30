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
        
        // Castling results
        VALID_CASTLE_KINGSIDE,
        VALID_CASTLE_QUEENSIDE,
        INVALID_CASTLE_KING_MOVED,
        INVALID_CASTLE_ROOK_MOVED,
        INVALID_CASTLE_PATH_BLOCKED,
        INVALID_CASTLE_THROUGH_CHECK,
        INVALID_CASTLE_IN_CHECK
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

    // Utility function to convert result to string (useful for debugging)
    std::string resultToString(MoveResult result) const;

private:
    // Helper function for boundary checking
    bool isValidSquare(int rank, int file) const;
    
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

    // Mapping of MoveResult to descriptive strings
    std::map<MoveResult, std::string> resultStrings = {
        {MoveResult::VALID, "Valid move"},
        {MoveResult::INVALID_OUT_OF_BOUNDS, "Move is out of bounds"},
        {MoveResult::INVALID_SAME_POSITION, "Cannot move to the same position"},
        {MoveResult::INVALID_WRONG_TURN, "Not your turn"},
        {MoveResult::INVALID_ILLEGAL_MOVE, "Illegal move"},
        
        // Castling messages
        {MoveResult::VALID_CASTLE_KINGSIDE, "Valid kingside castle"},
        {MoveResult::VALID_CASTLE_QUEENSIDE, "Valid queenside castle"},
        {MoveResult::INVALID_CASTLE_KING_MOVED, "Cannot castle - king has moved"},
        {MoveResult::INVALID_CASTLE_ROOK_MOVED, "Cannot castle - rook has moved"},
        {MoveResult::INVALID_CASTLE_PATH_BLOCKED, "Cannot castle - path is blocked"},
        {MoveResult::INVALID_CASTLE_THROUGH_CHECK, "Cannot castle through check"},
        {MoveResult::INVALID_CASTLE_IN_CHECK, "Cannot castle while in check"}
    };
};