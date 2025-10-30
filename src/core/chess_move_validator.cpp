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
    
    switch (piece) 
    {
        case Piece::WHITE_PAWN:
        case Piece::BLACK_PAWN:
            return validatePawnMove(logic, fromRank, fromFile, toRank, toFile);
        case Piece::WHITE_ROOK:
        case Piece::BLACK_ROOK:
            return validateRookMove(logic, fromRank, fromFile, toRank, toFile);
        case Piece::WHITE_KNIGHT:
        case Piece::BLACK_KNIGHT:
            return validateKnightMove(logic, fromRank, fromFile, toRank, toFile);
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
    if (toFile == fromFile) 
    {
        // One square forward
        if (toRank == fromRank + direction && logic.isSquareEmpty(toRank, toFile)) 
        {
            return true;
        }
        // Two squares forward from starting position
        if (fromRank == startRank && toRank == fromRank + 2 * direction &&
            logic.isSquareEmpty(fromRank + direction, toFile) &&
            logic.isSquareEmpty(toRank, toFile)) 
        {
            return true;
        }
    }
    // Capture move
    else if (abs(toFile - fromFile) == 1 && toRank == fromRank + direction) 
    {
        if (!logic.isSquareEmpty(toRank, toFile) &&
            !logic.areSameColorPieces(fromRank, fromFile, toRank, toFile)) 
        {
            return true;
        }
    }

    // Invalid pawn move
    return false;
}

bool ChessMoveValidator::validateRookMove(
    const ChessLogic& logic, 
    int fromRank, 
    int fromFile, 
    int toRank, 
    int toFile) const 
{
    // Rook moves in straight lines: either same rank or same file
    if (fromRank != toRank && fromFile != toFile) 
    {
        return false; // Not a straight line
    }

    // Check if path is clear
    int rankStep = (toRank > fromRank) ? 1 : (toRank < fromRank) ? -1 : 0;
    int fileStep = (toFile > fromFile) ? 1 : (toFile < fromFile) ? -1 : 0;

    int currentRank = fromRank + rankStep;
    int currentFile = fromFile + fileStep;

    while (currentRank != toRank || currentFile != toFile) 
    {
        if (!logic.isSquareEmpty(currentRank, currentFile)) 
        {
            return false; // Path is blocked
        }
        currentRank += rankStep;
        currentFile += fileStep;
    }

    // Check destination square
    if (!logic.isSquareEmpty(toRank, toFile) &&
        logic.areSameColorPieces(fromRank, fromFile, toRank, toFile)) 
    {
        return false; // Cannot capture own piece
    }

    return true; // Valid rook move
}

bool ChessMoveValidator::validateKnightMove(
    const ChessLogic& logic, 
    int fromRank, 
    int fromFile, 
    int toRank, 
    int toFile) const 
{
    int rankDiff = abs(toRank - fromRank);
    int fileDiff = abs(toFile - fromFile);

    // Knight moves in L-shape: 2 in one direction and 1 in the other
    if (!((rankDiff == 2 && fileDiff == 1) || (rankDiff == 1 && fileDiff == 2))) 
    {
        return false; // Not a valid knight move
    }

    // Check destination square
    if (!logic.isSquareEmpty(toRank, toFile) &&
        logic.areSameColorPieces(fromRank, fromFile, toRank, toFile)) 
    {
        return false; // Cannot capture own piece
    }

    return true; // Valid knight move
}

