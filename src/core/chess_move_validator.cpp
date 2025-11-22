#include "chess_move_validator.h"
#include <cmath> // For abs() function

// Namespace and type aliases for cleaner code
namespace BoardCfg = Config::Board;

ChessMoveValidator::MoveResult ChessMoveValidator::validateMove(
    const ChessBoard& board,
    const ChessGameState& gameState,
    const ChessMove& move) const {
    
    // 1. Basic move validation (bounds, turn, destination)
    BasicMoveValidator::ValidationResult basicResult = basicValidator.validateBasics(board, gameState, move);
    if (basicResult != BasicMoveValidator::ValidationResult::VALID) {
        return convertBasicResult(basicResult);
    }

    // 2. Validate piece-specific movement rules
    if (!pieceValidator.validatePieceMovement(board, gameState, move)) {
        return MoveResult::INVALID_ILLEGAL_MOVE;
    }

    // 3. Validate the player's king is not left in check
    if (checkValidator.wouldLeaveKingInCheck(board, gameState, move)) {
        return MoveResult::INVALID_ILLEGAL_MOVE;
    }

    // 4. Determine the type of valid move for return value
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destFile = move.getDestFile();
    char movingPiece = board.getPieceAt(srcRank, srcFile);
    
    // Check for castling (king-specific move)
    if (board.isKing(movingPiece)) {
        // If king is trying to move 2 squares horizontally, it MUST be castling
        int rankDiff = abs(move.getDestRank() - move.getSrcRank());
        int fileDiff = abs(move.getDestFile() - move.getSrcFile());
        if (rankDiff == 0 && fileDiff == 2) {
            // This is a castling attempt - validate it
            if (specialValidator.validateCastling(board, gameState, move)) {
                bool isKingside = (destFile > srcFile);
                return isKingside ? MoveResult::VALID_CASTLE_KINGSIDE : MoveResult::VALID_CASTLE_QUEENSIDE;
            } else {
                // Castling attempt failed - this is not a valid move
                return MoveResult::INVALID_ILLEGAL_MOVE;
            }
        }
    }

    // Check for en passant (pawn-specific move)
    if (board.isPawn(movingPiece) && 
            specialValidator.validateEnPassant(board, gameState, move)) {
        return MoveResult::VALID_EN_PASSANT;
    }
    
    // Check for promotion (pawn-specific move)
    if (board.isPawn(movingPiece) && 
            specialValidator.validatePromotion(board, gameState, move)) {
        return MoveResult::VALID_PROMOTION;
    }

    // If we get here, the move is a standard valid move
    return MoveResult::VALID;
}

bool ChessMoveValidator::wouldLeaveKingInCheck(
    const ChessBoard& board, 
    const ChessGameState& gameState,
    const ChessMove& move) const {
    // Get player and opponent
    char currentPlayer = gameState.getCurrentPlayer();
    char opponent = 
        (currentPlayer == 'w') ? 
        'b' : 
        'w';
    
    // Create a copy to test the move (efficient for standard chess board)
    ChessBoard testBoard = board;
    
    // Make the temporary move
    testBoard.makeTemporaryMove(move);
    
    // Find the king position after the move
    std::pair<int, int> kingPos = testBoard.getKingPosition(currentPlayer);
    
    // Check if the king is under attack
    return isSquareUnderAttack(testBoard, gameState, kingPos.first, kingPos.second, opponent);
}

bool ChessMoveValidator::isSquareUnderAttack(
    const ChessBoard& board, 
    const ChessGameState& gameState,
    const int defRank, 
    const int defFile, 
    const char attackingPlayer) const {
    
    return checkValidator.isSquareUnderAttack(
            board, gameState, defRank, defFile, attackingPlayer);
}

bool ChessMoveValidator::isValidMoveResult(const MoveResult result) const {
    return (result == MoveResult::VALID ||
            result == MoveResult::VALID_CASTLE_KINGSIDE ||
            result == MoveResult::VALID_CASTLE_QUEENSIDE ||
            result == MoveResult::VALID_EN_PASSANT ||
            result == MoveResult::VALID_PROMOTION);
}

bool ChessMoveValidator::isInvalidMoveResult(MoveResult result) const {
    return !isValidMoveResult(result);
}

ChessMoveValidator::MoveResult ChessMoveValidator::convertBasicResult(BasicMoveValidator::ValidationResult result) const {
    switch (result) {
        case BasicMoveValidator::ValidationResult::VALID:
            return ChessMoveValidator::MoveResult::VALID;
        case BasicMoveValidator::ValidationResult::INVALID_OUT_OF_BOUNDS:
            return ChessMoveValidator::MoveResult::INVALID_OUT_OF_BOUNDS;
        case BasicMoveValidator::ValidationResult::INVALID_SAME_POSITION:
            return ChessMoveValidator::MoveResult::INVALID_SAME_POSITION;
        case BasicMoveValidator::ValidationResult::INVALID_NO_PIECE:
            return ChessMoveValidator::MoveResult::INVALID_NO_PIECE;
        case BasicMoveValidator::ValidationResult::INVALID_WRONG_TURN:
            return ChessMoveValidator::MoveResult::INVALID_WRONG_TURN;
        case BasicMoveValidator::ValidationResult::INVALID_DESTINATION:
            return ChessMoveValidator::MoveResult::INVALID_ILLEGAL_MOVE;
        default:
            return ChessMoveValidator::MoveResult::INVALID_ILLEGAL_MOVE;
    }
}