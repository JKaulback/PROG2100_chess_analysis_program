#include "special_move_validator.h"
#include "check_validator.h"
#include "piece_movement_validator.h"
#include "../../config/config.h"
#include <cmath>

namespace BoardCfg = Config::Board;

class CheckValidator;

bool SpecialMoveValidator::validateCastling(
    const ChessBoard& board,
    const ChessGameState& gameState,
    const ChessMove& move) const {
    
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destRank = move.getDestRank();
    int destFile = move.getDestFile();
    
    // Get the player attempting to castle
    char currentPlayer = gameState.getCurrentPlayer();
    
    // Check if this is a king making a castling move
    char piece = board.getPieceAt(srcRank, srcFile);
    if (!board.isKing(piece))
        return false; // Not a king move

    // Check if the move is a potential castling move (king moves 2 squares horizontally)
    int rankDiff = abs(destRank - srcRank);
    int fileDiff = abs(destFile - srcFile);
    if (rankDiff != 0 || fileDiff != 2) 
        return false; // Not a castling move pattern
    
    // Determine if it's kingside or queenside
    bool isKingside = (destFile > srcFile);
    
    return canCastle(board, gameState, currentPlayer, isKingside);
}

bool SpecialMoveValidator::canCastle(
    const ChessBoard& board,
    const ChessGameState& gameState, 
    const char player, 
    const bool isKingside) const {
    
    // Check basic castling rights
    if (isKingside && !gameState.canCastleKingside(player)) 
        return false;
    if (!isKingside && !gameState.canCastleQueenside(player)) 
        return false;
    
    // Get king and rook positions
    int kingRank = 
        (player == 'w') ? 
        BoardCfg::WHITE_BACK_RANK : 
        BoardCfg::BLACK_BACK_RANK;
    int kingFile = BoardCfg::KING_FILE;
    int rookFile = isKingside ? 
        BoardCfg::KINGSIDE_ROOK_FILE : 
        BoardCfg::QUEENSIDE_ROOK_FILE;
    
    // Check if king is currently in check
    char opponent = 
        (player == 'w') ? 
        'b' :
        'w';
    
    CheckValidator checkValidator;
    if (checkValidator.isSquareUnderAttack(board, gameState, kingRank, kingFile, opponent)) 
        return false; // Cannot castle while in check
    
    // Check if path is clear
    int startFile = 
        (kingFile < rookFile) ? 
        kingFile + 1 : 
        rookFile + 1;
    int endFile = 
        (kingFile < rookFile) ? 
        rookFile - 1 : 
        kingFile - 1;
    
    for (int file = startFile; file <= endFile; ++file) {
        if (!board.isSquareEmpty(kingRank, file)) 
            return false; // Path is blocked
    }
    
    // Check if king passes through check
    int direction = isKingside ? 1 : -1;
    for (int i = 1; i <= 2; ++i) {
        int testFile = kingFile + (i * direction);
        if (checkValidator.isSquareUnderAttack(board, gameState, kingRank, testFile, opponent)) 
            return false; // Cannot castle through check
    }
    
    return true; // All castling conditions met
}

bool SpecialMoveValidator::validateEnPassant(
    const ChessBoard& board,
    const ChessGameState& gameState,
    const ChessMove& move) const {
    
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destRank = move.getDestRank();
    int destFile = move.getDestFile();
    
    // Check if en passant is available
    if (!gameState.isEnPassantAvailable()) 
        return false;
    
    // Check if the destination is the en passant target square
    if (!gameState.isEnPassantTarget(destRank, destFile)) 
        return false;
    
    // Get the en passant pawn position
    std::pair<int, int> enPassantPawn = gameState.getEnPassantPawn();
    int pawnRank = enPassantPawn.first;
    int pawnFile = enPassantPawn.second;
    
    // Verify the en passant pawn exists and is the opponent's pawn
    char targetPawn = board.getPieceAt(pawnRank, pawnFile);
    if (targetPawn == BoardCfg::EMPTY) 
        return false;
    
    // Verify it's an opponent's pawn
    char movingPiece = board.getPieceAt(srcRank, srcFile);
    if (board.areSameColorPieces(ChessMove{srcRank, srcFile, pawnRank, pawnFile})) 
        return false; // Cannot capture your own pawn
    
    // Verify the capturing pawn is on the correct rank for en passant
    int expectedRank = 
        (movingPiece == 'P') ? 
        BoardCfg::WHITE_EN_PASSANT_CAPTURE_RANK : 
        BoardCfg::BLACK_EN_PASSANT_CAPTURE_RANK;
    if (srcRank != expectedRank) 
        return false;
    
    return true; // Valid en passant capture
}

bool SpecialMoveValidator::validatePromotion(
    const ChessBoard& board, 
    const ChessGameState& gameState,
    const ChessMove& move) const {
        
    int destRank = move.getDestRank();
    
    // Get the piece being promoted
    char piece = board.getPieceAt(move.getSrcRank(), move.getSrcFile());
    
    // Determine promotion rank based on pawn color
    int promotionRank = 
        (piece == 'P') ? 
        BoardCfg::WHITE_PROMOTES_AT_RANK : 
        BoardCfg::BLACK_PROMOTES_AT_RANK;
    
    // Check if the pawn is moving to the promotion rank
    if (destRank != promotionRank) 
        return false; // Not moving to promotion rank
    
    // Check if the move is a valid pawn move to the promotion rank
    PieceMovementValidator pieceValidator;
    return pieceValidator.validatePawnMove(board, gameState, move);
}
