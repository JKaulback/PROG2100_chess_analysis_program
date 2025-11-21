#include "basic_move_validator.h"
#include "../../config/config.h"

namespace BoardCfg = Config::Board;

BasicMoveValidator::ValidationResult BasicMoveValidator::validateBasics(
    const ChessBoard& board,
    const ChessGameState& gameState,
    const ChessMove& move) const {
    
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destRank = move.getDestRank();
    int destFile = move.getDestFile();
    
    // 1. Check if source and destination are within board boundaries
    if (!board.isValidSquare(srcRank, srcFile) || 
            !board.isValidSquare(destRank, destFile)) 
        return ValidationResult::INVALID_OUT_OF_BOUNDS;
    
    // 2. Check if moving to the same position
    if (srcRank == destRank && srcFile == destFile) 
        return ValidationResult::INVALID_SAME_POSITION;
    
    // 3. Check if there's a piece to move
    char piece = board.getPieceAt(srcRank, srcFile);
    if (piece == BoardCfg::EMPTY) 
        return ValidationResult::INVALID_NO_PIECE;
    
    // 4. Check if it's the correct player's turn
    char currentPlayer = gameState.getCurrentPlayer();
    if ((currentPlayer == 'w' && 
            !board.isWhitePiece(srcRank, srcFile)) || 
            (currentPlayer == 'b' && 
            board.isWhitePiece(srcRank, srcFile))) 
        return ValidationResult::INVALID_WRONG_TURN;
    
    // 5. Check destination square validity
    if (!checkDestinationSquare(board, move)) 
        return ValidationResult::INVALID_DESTINATION;

    return ValidationResult::VALID;
}

bool BasicMoveValidator::checkDestinationSquare(
    const ChessBoard& board, 
    const ChessMove& move) const {
        
    // True if the destination square is empty or contains an opposing piece
    return (board.isSquareEmpty(move.getDestRank(), move.getDestFile()) || 
        !board.areSameColorPieces(move));
}
