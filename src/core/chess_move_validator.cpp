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
        return MoveResult::INVALID_NO_PIECE;
    }
    
    // 4. Check if it's the correct player's turn
    Player currentPlayer = logic.getCurrentPlayer();
    if ((currentPlayer == Player::WHITE_PLAYER && !logic.isWhitePiece(fromRank, fromFile)) ||
        (currentPlayer == Player::BLACK_PLAYER && !logic.isBlackPiece(fromRank, fromFile))) {
        return MoveResult::INVALID_WRONG_TURN;
    }
    
    // 5. Validate piece-specific movement rules
    if (!validatePieceMovement(logic, fromRank, fromFile, toRank, toFile)) {
        return MoveResult::INVALID_ILLEGAL_MOVE;
    }

    // If we get here, the move is valid
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

bool ChessMoveValidator::validatePieceMovement(
    const ChessLogic& logic, 
    int fromRank, 
    int fromFile, 
    int toRank, 
    int toFile) const 
{
    Piece piece = logic.getPieceAt(fromRank, fromFile);
    
    switch (piece) {
        case Piece::WHITE_PAWN:
        case Piece::BLACK_PAWN:
            return validatePawnMove(logic, fromRank, fromFile, toRank, toFile);
        // Future: Add cases for other pieces (Rook, Knight, Bishop, Queen, King)
        default:
            return false; // For now, unhandled pieces are invalid
    }
}

bool ChessMoveValidator::validatePawnMove(
    const ChessLogic& logic, 
    int fromRank, 
    int fromFile, 
    int toRank, 
    int toFile) const 
{
    Piece piece = logic.getPieceAt(fromRank, fromFile);
    int direction = (piece == Piece::WHITE_PAWN) ? 1 : -1; // White moves up, Black moves down
    int startRank = (piece == Piece::WHITE_PAWN) ? 1 : 6; // Starting ranks for pawns

    // Standard move forward
    if (toFile == fromFile) {
        // One square forward
        if (toRank == fromRank + direction && logic.isSquareEmpty(toRank, toFile)) {
            return true;
        }
        // Two squares forward from starting position
        if (fromRank == startRank && toRank == fromRank + 2 * direction &&
            logic.isSquareEmpty(fromRank + direction, toFile) &&
            logic.isSquareEmpty(toRank, toFile)) {
            return true;
        }
    }
    // Capture move
    else if (abs(toFile - fromFile) == 1 && toRank == fromRank + direction) {
        if (!logic.isSquareEmpty(toRank, toFile) &&
            !logic.areSameColorPieces(fromRank, fromFile, toRank, toFile)) {
            return true;
        }
    }

    // Invalid pawn move
    return false;
}