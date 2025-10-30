#include "chess_move_validator.h"
#include "chess_logic.h"

// Namespace and type aliases for cleaner code
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
    
    // 3. Check if there's a piece to move
    ChessLogic::Piece piece = logic.getPieceAt(fromRank, fromFile);
    if (piece == ChessLogic::Piece::EMPTY) {
        return MoveResult::INVALID_NO_PIECE;
    }
    
    // 4. Check if it's the correct player's turn
    ChessLogic::Player currentPlayer = logic.getCurrentPlayer();
    if ((currentPlayer == ChessLogic::Player::WHITE_PLAYER && !logic.isWhitePiece(fromRank, fromFile)) ||
        (currentPlayer == ChessLogic::Player::BLACK_PLAYER && !logic.isBlackPiece(fromRank, fromFile))) {
        return MoveResult::INVALID_WRONG_TURN;
    }
    
    // 5. Check destination square validity
    if (!checkDestinationSquare(logic, fromRank, fromFile, toRank, toFile)) {
        return MoveResult::INVALID_ILLEGAL_MOVE;
    }

    // 6. Validate piece-specific movement rules
    if (!validatePieceMovement(logic, fromRank, fromFile, toRank, toFile)) {
        return MoveResult::INVALID_ILLEGAL_MOVE;
    }

    // 7. Validate the player's king is not left in check
    if (wouldLeaveKingInCheck(logic, fromRank, fromFile, toRank, toFile)) {
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
    ChessLogic::Piece piece = logic.getPieceAt(fromRank, fromFile);
    
    // Special case for pawns - they have unique movement rules
    if (piece == ChessLogic::Piece::WHITE_PAWN || piece == ChessLogic::Piece::BLACK_PAWN) {
        return validatePawnMove(logic, fromRank, fromFile, toRank, toFile);
    }
    
    // For all other pieces, use the unified basic movement validation
    return validateBasicPieceMovement(logic, piece, fromRank, fromFile, toRank, toFile);
}

bool ChessMoveValidator::checkDestinationSquare(
    const ChessLogic& logic, 
    int fromRank, 
    int fromFile, 
    int toRank, 
    int toFile) const 
{
    // Check if destination square is occupied by same color piece
    if (!logic.isSquareEmpty(toRank, toFile) &&
        logic.areSameColorPieces(fromRank, fromFile, toRank, toFile)) 
    {
        return false; // Cannot capture own piece
    }
    return true; // Destination square is valid
}

bool ChessMoveValidator::isPathClearForSlidingPiece(
    const ChessLogic& logic, 
    int fromRank, 
    int fromFile, 
    int toRank, 
    int toFile) const 
{
    // Calculate the direction of movement
    int rankStep = (toRank > fromRank) ? 1 : (toRank < fromRank) ? -1 : 0;
    int fileStep = (toFile > fromFile) ? 1 : (toFile < fromFile) ? -1 : 0;

    // Start checking from the square after the starting position
    int currentRank = fromRank + rankStep;
    int currentFile = fromFile + fileStep;

    // Check each square along the path (excluding destination)
    while (currentRank != toRank || currentFile != toFile) 
    {
        if (!logic.isSquareEmpty(currentRank, currentFile)) 
        {
            return false; // Path is blocked
        }
        currentRank += rankStep;
        currentFile += fileStep;
    }

    return true; // Path is clear
}

bool ChessMoveValidator::validatePawnMove(
    const ChessLogic& logic, 
    int fromRank, 
    int fromFile, 
    int toRank, 
    int toFile) const 
{
    ChessLogic::Piece piece = logic.getPieceAt(fromRank, fromFile);
    int direction = (piece == ChessLogic::Piece::WHITE_PAWN) ? 1 : -1; // White moves up, Black moves down
    int startRank = (piece == ChessLogic::Piece::WHITE_PAWN) ? 1 : 6; // Starting ranks for pawns

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

// Individual piece validation methods removed - now using validateBasicPieceMovement()

bool ChessMoveValidator::wouldLeaveKingInCheck(
    const ChessLogic& logic, 
    int fromRank, 
    int fromFile, 
    int toRank, 
    int toFile) const 
{
    // Get the current player (whose king we're checking)
    ChessLogic::Player currentPlayer = logic.getCurrentPlayer();
    ChessLogic::Player opponent = (currentPlayer == ChessLogic::Player::WHITE_PLAYER) 
                                  ? ChessLogic::Player::BLACK_PLAYER 
                                  : ChessLogic::Player::WHITE_PLAYER;
    
    // Create a copy to test the move (efficient for 8x8 board)
    ChessLogic testLogic = logic;
    
    // Make the temporary move
    testLogic.makeTemporaryMove(fromRank, fromFile, toRank, toFile);
    
    // Find the king position after the move
    std::pair<int, int> kingPos = testLogic.getKingPosition(currentPlayer);
    
    // Check if the king is under attack
    return isSquareUnderAttack(testLogic, kingPos.first, kingPos.second, opponent);
}

bool ChessMoveValidator::isSquareUnderAttack(
    const ChessLogic& logic, 
    int rank, 
    int file, 
    ChessLogic::Player attackingPlayer) const 
{
    // Check all squares on the board for pieces belonging to the attacking player
    for (int r = 0; r < 8; r++) 
    {
        for (int f = 0; f < 8; f++) 
        {
            ChessLogic::Piece piece = logic.getPieceAt(r, f);
            
            // Skip empty squares
            if (piece == ChessLogic::Piece::EMPTY) continue;
            
            // Check if this piece belongs to the attacking player
            bool isAttackersPiece = false;
            if (attackingPlayer == ChessLogic::Player::WHITE_PLAYER) 
            {
                isAttackersPiece = logic.isWhitePiece(r, f);
            } else {
                isAttackersPiece = logic.isBlackPiece(r, f);
            }
            
            if (!isAttackersPiece) continue;
            
            // Check if this piece can attack the target square
            // For pawns, special logic since they attack differently than they move
            if (piece == ChessLogic::Piece::WHITE_PAWN || piece == ChessLogic::Piece::BLACK_PAWN) 
            {
                int direction = (piece == ChessLogic::Piece::WHITE_PAWN) ? 1 : -1;
                if (rank == r + direction && abs(file - f) == 1) 
                {
                    return true; // Pawn can attack diagonally
                }
            } else 
            {
                // For other pieces, use existing movement validation
                // but skip friendly fire check since we're checking attacks
                if (validateBasicPieceMovement(logic, piece, r, f, rank, file)) 
                {
                    return true;
                }
            }
        }
    }
    
    return false; // No piece can attack this square
}

bool ChessMoveValidator::validateBasicPieceMovement(
    const ChessLogic& logic, 
    ChessLogic::Piece piece, 
    int fromRank, 
    int fromFile, 
    int toRank, 
    int toFile) const 
{
    // This validates piece movement without checking destination square ownership
    // Used for attack detection where we want to know if a piece CAN reach a square
    
    switch (piece) {
        case ChessLogic::Piece::WHITE_ROOK:
        case ChessLogic::Piece::BLACK_ROOK:
            return (fromRank == toRank || fromFile == toFile) && 
                   isPathClearForSlidingPiece(logic, fromRank, fromFile, toRank, toFile);
        
        case ChessLogic::Piece::WHITE_BISHOP:
        case ChessLogic::Piece::BLACK_BISHOP: {
            int rankDiff = abs(toRank - fromRank);
            int fileDiff = abs(toFile - fromFile);
            return (rankDiff == fileDiff) && 
                   isPathClearForSlidingPiece(logic, fromRank, fromFile, toRank, toFile);
        }
        
        case ChessLogic::Piece::WHITE_QUEEN:
        case ChessLogic::Piece::BLACK_QUEEN: {
            int rankDiff = abs(toRank - fromRank);
            int fileDiff = abs(toFile - fromFile);
            return ((fromRank == toRank || fromFile == toFile || rankDiff == fileDiff) && 
                    isPathClearForSlidingPiece(logic, fromRank, fromFile, toRank, toFile));
        }
        
        case ChessLogic::Piece::WHITE_KNIGHT:
        case ChessLogic::Piece::BLACK_KNIGHT: {
            int rankDiff = abs(toRank - fromRank);
            int fileDiff = abs(toFile - fromFile);
            return (rankDiff == 2 && fileDiff == 1) || (rankDiff == 1 && fileDiff == 2);
        }
        
        case ChessLogic::Piece::WHITE_KING:
        case ChessLogic::Piece::BLACK_KING: {
            int rankDiff = abs(toRank - fromRank);
            int fileDiff = abs(toFile - fromFile);
            return rankDiff <= 1 && fileDiff <= 1;
        }
        
        default:
            return false;
    }
}