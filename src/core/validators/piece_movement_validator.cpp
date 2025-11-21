#include "piece_movement_validator.h"
#include "../../config/config.h"
#include <cmath>

namespace BoardCfg = Config::Board;

bool PieceMovementValidator::validatePieceMovement(
    const ChessBoard& board, 
    const ChessGameState& gameState,
    const ChessMove& move) const {
    
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    char piece = board.getPieceAt(srcRank, srcFile);
    
    // Special case for pawns - they have unique movement rules
    if (board.isPawn(piece)) 
        return validatePawnMove(board, gameState, move);
    
    // For all other pieces, use the unified basic movement validation
    return validateBasicPieceMovement(board, gameState, piece, move);
}

bool PieceMovementValidator::isPathClearForSlidingPiece(
    const ChessBoard& board, 
    const ChessMove& move) const {
    
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destRank = move.getDestRank();
    int destFile = move.getDestFile();
    
    // Calculate the direction of movement
    int rankStep = 
        (destRank > srcRank) ? 
        1 : 
            (destRank < srcRank) ? 
            -1 : 
            0;
    int fileStep = 
        (destFile > srcFile) ? 
        1 : 
            (destFile < srcFile) ? 
            -1 : 
            0;

    // Start checking from the square after the starting position
    int currentRank = srcRank + rankStep;
    int currentFile = srcFile + fileStep;

    // Check each square along the path (excluding destination)
    while (currentRank != destRank || currentFile != destFile) {
        // Check if path is blocked
        if (!board.isSquareEmpty(currentRank, currentFile))
            return false;
        // Go to next square
        currentRank += rankStep;
        currentFile += fileStep;
    }

    return true; // Path is clear
}

bool PieceMovementValidator::validatePawnMove(
    const ChessBoard& board,
    const ChessGameState& gameState,
    const ChessMove& move) const {

    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destRank = move.getDestRank();
    int destFile = move.getDestFile();
    
    // Setup variables for checking
    char piece = board.getPieceAt(srcRank, srcFile);
    int direction = 
        (piece == 'P') ? 
        BoardCfg::WHITE_PAWN_DIRECTION : 
        BoardCfg::BLACK_PAWN_DIRECTION;
    int startRank = 
        (piece == 'P') ? 
        BoardCfg::WHITE_PAWN_START_RANK : 
        BoardCfg::BLACK_PAWN_START_RANK;

    // Standard move forward
    if (destFile == srcFile) {    
        // One square forward
        if (destRank == srcRank + direction && 
                board.isSquareEmpty(destRank, destFile)) 
            return true;
        // Two squares forward from starting position
        if (srcRank == startRank && destRank == srcRank + 2 * direction &&
                board.isSquareEmpty(srcRank + direction, destFile) &&
                board.isSquareEmpty(destRank, destFile)) 
            return true;
    }
    // Capture move (including en passant, diagonal check)
    else if (abs(destFile - srcFile) == 1 && destRank == srcRank + direction) { 
        // Regular capture
        if (!board.isSquareEmpty(destRank, destFile) &&
                !board.areSameColorPieces(move)) 
            return true;
        // En passant capture
        else if (board.isSquareEmpty(destRank, destFile) && 
                gameState.isEnPassantTarget(destRank, destFile)) 
            return true; // En passant validation handled by SpecialMoveValidator
    }
    
    // Invalid pawn move
    return false;
}

bool PieceMovementValidator::validateBasicPieceMovement(
    const ChessBoard& board, 
    const ChessGameState& gameState,
    const char piece,
    const ChessMove& move) const {
    
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destRank = move.getDestRank();
    int destFile = move.getDestFile();
    
    // This validates piece movement without checking destination square ownership
    // Used for attack detection where we want to know if a piece CAN reach a square
    switch (piece) {
        case 'R':
        case 'r':
            return (srcRank == destRank || srcFile == destFile) && // Horizontal and vertical
                isPathClearForSlidingPiece(board, move); // Sliding piece
        
        case 'B':
        case 'b':
            return (abs(destRank - srcRank) == abs(destFile - srcFile)) && // Diagonal 
                isPathClearForSlidingPiece(board, move); // Sliding piece
        
        case 'Q':
        case 'q': 
            return ((srcRank == destRank || srcFile == destFile || // Horizontal and vertical
                abs(destRank - srcRank) == abs(destFile - srcFile)) && // Diagonal
                isPathClearForSlidingPiece(board, move)); // Sliding piece
        
        case 'N':
        case 'n': 
            return (abs(destRank - srcRank) == 2 && abs(destFile - srcFile) == 1) ||  // Vertical L
                (abs(destRank - srcRank) == 1 && abs(destFile - srcFile) == 2); // Horizontal L
        
        case 'K':
        case 'k': { 
            // Normal king move (one square in any direction)
            if (abs(destRank - srcRank) <= 1 && abs(destFile - srcFile) <= 1) 
                return true;            
            // Castling move (king moves 2 squares horizontally) - handled by SpecialMoveValidator
            if (abs(destRank - srcRank) == 0 && abs(destFile - srcFile) == 2) 
                return true; // Basic movement pattern is valid, castling rules checked elsewhere
            
            return false;
        }
        default:
            return false;
    }
}
