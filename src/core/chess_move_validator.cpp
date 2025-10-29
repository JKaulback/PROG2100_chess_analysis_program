#include "chess_move_validator.h"
#include "chess_logic.h"

namespace BoardCfg = Config::Board;

ChessMoveValidator::MoveResult ChessMoveValidator::validateMove(
    const ChessLogic& logic,
    int fromRank, 
    int fromFile, 
    int toRank, 
    int toFile) const 
{
    // 1. Check if source and destination are within board boundaries
    if (!isValidSquare(fromRank, fromFile) || !isValidSquare(toRank, toFile)) {
        return MoveResult::INVALID_OUT_OF_BOUNDS;
    }
    
    // 2. Check if moving to the same position
    if (fromRank == toRank && fromFile == toFile) {
        return MoveResult::INVALID_SAME_POSITION;
    }
    
    // If we get here, the move is valid (at least for basic boundary checking)
    return MoveResult::VALID;
}

std::string ChessMoveValidator::resultToString(MoveResult result) const 
{
    return resultStrings.at(result);
}

bool ChessMoveValidator::isValidSquare(int rank, int file) const 
{
    return rank >= BoardCfg::MIN_RANK && rank <= BoardCfg::MAX_RANK &&
           file >= BoardCfg::MIN_FILE && file <= BoardCfg::MAX_FILE;
}