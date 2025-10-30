#pragma once

#include <string>
#include <map>
#include "../config/config.h"

// Forward declaration to avoid circular dependencies
class ChessLogic;

class ChessMoveValidator {
public:
    // Simple validation result enum - start with basic cases
    enum class MoveResult {
        VALID,
        INVALID_OUT_OF_BOUNDS,
        INVALID_SAME_POSITION,
        INVALID_NO_PIECE,
        INVALID_WRONG_TURN,
        INVALID_ILLEGAL_MOVE
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

    // Helper function for sliding pieces (rook, bishop, queen)
    bool isPathClearForSlidingPiece(const ChessLogic& logic, int fromRank, int fromFile, int toRank, int toFile) const;

    // Piece specific validation
    bool validatePawnMove(const ChessLogic& logic, int fromRank, int fromFile, int toRank, int toFile) const;
    bool validateRookMove(const ChessLogic& logic, int fromRank, int fromFile, int toRank, int toFile) const;
    bool validateKnightMove(const ChessLogic& logic, int fromRank, int fromFile, int toRank, int toFile) const;
    bool validateBishopMove(const ChessLogic& logic, int fromRank, int fromFile, int toRank, int toFile) const;
    bool validateKingMove(const ChessLogic& logic, int fromRank, int fromFile, int toRank, int toFile) const;
    bool validateQueenMove(const ChessLogic& logic, int fromRank, int fromFile, int toRank, int toFile) const;

    // Mapping of MoveResult to descriptive strings
    std::map<MoveResult, std::string> resultStrings = {
        {MoveResult::VALID, "Valid move"},
        {MoveResult::INVALID_OUT_OF_BOUNDS, "Move is out of bounds"},
        {MoveResult::INVALID_SAME_POSITION, "Cannot move to the same position"},
        {MoveResult::INVALID_WRONG_TURN, "Not your turn"}
    };
};