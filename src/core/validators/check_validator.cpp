#include "check_validator.h"
#include "piece_movement_validator.h"
#include "../../config/config.h"
#include <cmath>

namespace BoardCfg = Config::Board;

bool CheckValidator::wouldLeaveKingInCheck(
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

bool CheckValidator::isSquareUnderAttack(
    const ChessBoard& board, 
    const ChessGameState& gameState,
    const int defRank, 
    const int defFile, 
    const char attackingPlayer) const {
    
    // Check all squares on the board for pieces belonging to the attacking player
    for (int atkRank = BoardCfg::MIN_RANK; atkRank <= BoardCfg::MAX_RANK; atkRank++) {
        for (int atkFile = BoardCfg::MIN_FILE; atkFile <= BoardCfg::MAX_FILE; atkFile++) {
            char attackingPiece = board.getPieceAt(atkRank, atkFile);
            
            // Skip empty squares
            if (attackingPiece == BoardCfg::EMPTY) 
                continue;
            
            // Check if this piece belongs to the attacking player
            bool isAttackersPiece = 
                (attackingPlayer == 'w') ?
                board.isWhitePiece(atkRank, atkFile) :
                !board.isWhitePiece(atkRank, atkFile);
            
            if (!isAttackersPiece) 
                continue;
            
            // Check if this piece can attack the target square
            // For pawns, special logic since they attack differently than they move
            if (board.isPawn(attackingPiece)) {
                int direction =
                    (attackingPiece == 'P') ? 
                    BoardCfg::WHITE_PAWN_DIRECTION : 
                    BoardCfg::BLACK_PAWN_DIRECTION;
                
                if (defRank == atkRank + direction && abs(defFile - atkFile) == 1) 
                    return true; // Pawn can attack diagonally
            } else {
                // For other pieces, use existing movement validation
                PieceMovementValidator pieceValidator;
                if (pieceValidator.validateBasicPieceMovement(board, gameState, attackingPiece, ChessMove{atkRank, atkFile, defRank, defFile}))
                    return true;
            }
        }
    }
    return false; // No piece can attack this square
}
