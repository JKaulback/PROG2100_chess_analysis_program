#include "chess_move_validator.h"
#include "chess_logic.h"

// Namespace and type aliases for cleaner code
namespace BoardCfg = Config::Board;
using Player = ChessLogic::Player;
using Piece = ChessLogic::Piece;

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
    
    // 3. Check if there's a piece to move
    Piece piece = logic.getPieceAt(fromRank, fromFile);
    if (piece == Piece::EMPTY) {
        return MoveResult::INVALID_OUT_OF_BOUNDS; // Reusing this for now, could add INVALID_NO_PIECE
    }
    
    // 4. Check if it's the correct player's turn
    Player currentPlayer = logic.getCurrentPlayer();
    if ((currentPlayer == Player::WHITE_PLAYER && !logic.isWhitePiece(fromRank, fromFile)) ||
        (currentPlayer == Player::BLACK_PLAYER && !logic.isBlackPiece(fromRank, fromFile))) {
        return MoveResult::INVALID_WRONG_TURN;
    }
    
    // If we get here, the move is valid (at least for basic validation)
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