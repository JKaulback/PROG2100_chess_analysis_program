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
        INVALID_SAME_POSITION
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

    std::map<MoveResult, std::string> resultStrings = {
        {MoveResult::VALID, "Valid move"},
        {MoveResult::INVALID_OUT_OF_BOUNDS, "Move is out of bounds"},
        {MoveResult::INVALID_SAME_POSITION, "Cannot move to the same position"}
    };
};