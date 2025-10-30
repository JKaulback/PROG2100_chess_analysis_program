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
    if (!logic.isValidSquare(fromRank, fromFile) || !logic.isValidSquare(toRank, toFile)) 
    {
        return MoveResult::INVALID_OUT_OF_BOUNDS;
    }
    
    // 2. Check if moving to the same position
    if (fromRank == toRank && fromFile == toFile) 
    {
        return MoveResult::INVALID_SAME_POSITION;
    }
    
    // 3. Check if there's a piece to move
    ChessLogic::Piece piece = logic.getPieceAt(fromRank, fromFile);
    if (piece == ChessLogic::Piece::EMPTY) 
    {
        return MoveResult::INVALID_NO_PIECE;
    }
    
    // 4. Check if it's the correct player's turn
    ChessLogic::Player currentPlayer = logic.getCurrentPlayer();
    if ((currentPlayer == ChessLogic::Player::WHITE_PLAYER && !logic.isWhitePiece(fromRank, fromFile)) ||
        (currentPlayer == ChessLogic::Player::BLACK_PLAYER && !logic.isBlackPiece(fromRank, fromFile))) 
        {
        return MoveResult::INVALID_WRONG_TURN;
    }
    
    // 5. Check destination square validity
    if (!checkDestinationSquare(logic, fromRank, fromFile, toRank, toFile)) 
    {
        return MoveResult::INVALID_ILLEGAL_MOVE;
    }

    // 6. Check for special moves
    if (validateCastling(logic, fromRank, fromFile, toRank, toFile)) 
    {
        bool isKingside = (toFile > fromFile);
        return isKingside ? MoveResult::VALID_CASTLE_KINGSIDE : MoveResult::VALID_CASTLE_QUEENSIDE;
    }

    // Check for en passant attempt
    if (validateEnPassant(logic, fromRank, fromFile, toRank, toFile)) 
    {
        return MoveResult::VALID_EN_PASSANT;
    }
    
    // 7. Validate piece-specific movement rules
    if (!validatePieceMovement(logic, fromRank, fromFile, toRank, toFile)) 
    {
        return MoveResult::INVALID_ILLEGAL_MOVE;
    }

    // 8. Validate the player's king is not left in check
    if (wouldLeaveKingInCheck(logic, fromRank, fromFile, toRank, toFile)) 
    {
        return MoveResult::INVALID_ILLEGAL_MOVE;
    }

    // 9. Check for promotion
    if (validatePromotion(logic, fromRank, fromFile, toRank, toFile)) 
    {
        return MoveResult::VALID_PROMOTION;
    }

    // If we get here, the move is valid
    return MoveResult::VALID;
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
    if (piece == ChessLogic::Piece::WHITE_PAWN || piece == ChessLogic::Piece::BLACK_PAWN) 
    {
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
    int startRank = (piece == ChessLogic::Piece::WHITE_PAWN) ? BoardCfg::WHITE_PAWN_START_RANK : BoardCfg::BLACK_PAWN_START_RANK;

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
    // Capture move (including en passant)
    else if (abs(toFile - fromFile) == 1 && toRank == fromRank + direction) 
    {
        // Regular capture
        if (!logic.isSquareEmpty(toRank, toFile) &&
            !logic.areSameColorPieces(fromRank, fromFile, toRank, toFile)) 
        {
            return true;
        }
        // En passant capture
        if (logic.isSquareEmpty(toRank, toFile) && logic.isEnPassantTarget(toRank, toFile)) 
        {
            return validateEnPassant(logic, fromRank, fromFile, toRank, toFile);
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

bool ChessMoveValidator::validateCastling(const ChessLogic& logic, int fromRank, int fromFile, int toRank, int toFile) const {
    ChessLogic::Player currentPlayer = logic.getCurrentPlayer();
    
    // Check if this is a king making a castling move
    ChessLogic::Piece piece = logic.getPieceAt(fromRank, fromFile);
    if (piece != ChessLogic::Piece::WHITE_KING && piece != ChessLogic::Piece::BLACK_KING) 
    {
        return false; // Not a king move
    }
    
    // Check if the move is a potential castling move (king moves 2 squares horizontally)
    int rankDiff = abs(toRank - fromRank);
    int fileDiff = abs(toFile - fromFile);
    if (rankDiff != 0 || fileDiff != 2) 
    {
        return false; // Not a castling move pattern
    }
    
    // Determine if it's kingside or queenside
    bool isKingside = (toFile > fromFile);
    
    return canCastle(logic, currentPlayer, isKingside);
}

bool ChessMoveValidator::canCastle(const ChessLogic& logic, ChessLogic::Player player, bool isKingside) const {
    // Check basic castling rights
    if (isKingside && !logic.canCastleKingside(player)) 
    {
        return false;
    }
    if (!isKingside && !logic.canCastleQueenside(player)) 
    {
        return false;
    }
    
    // Get king and rook positions
    int kingRank = (player == ChessLogic::Player::WHITE_PLAYER) ? 0 : 7;
    int kingFile = 4;
    int rookFile = isKingside ? 7 : 0;
    
    // Check if king is currently in check
    ChessLogic::Player opponent = (player == ChessLogic::Player::WHITE_PLAYER) 
                                  ? ChessLogic::Player::BLACK_PLAYER 
                                  : ChessLogic::Player::WHITE_PLAYER;
    
    if (isSquareUnderAttack(logic, kingRank, kingFile, opponent)) 
    {
        return false; // Cannot castle while in check
    }
    
    // Check if path is clear
    int startFile = (kingFile < rookFile) ? kingFile + 1 : rookFile + 1;
    int endFile = (kingFile < rookFile) ? rookFile - 1 : kingFile - 1;
    
    for (int file = startFile; file <= endFile; ++file) 
    {
        if (!logic.isSquareEmpty(kingRank, file)) 
        {
            return false; // Path is blocked
        }
    }
    
    // Check if king passes through check
    int direction = isKingside ? 1 : -1;
    for (int i = 1; i <= 2; ++i) {
        int testFile = kingFile + (i * direction);
        if (isSquareUnderAttack(logic, kingRank, testFile, opponent)) 
        {
            return false; // Cannot castle through check
        }
    }
    
    return true; // All castling conditions met
}

bool ChessMoveValidator::validateEnPassant(const ChessLogic& logic, int fromRank, int fromFile, int toRank, int toFile) const {
    // Check if en passant is available
    if (!logic.isEnPassantAvailable()) 
    {
        return false;
    }
    
    // Check if the destination is the en passant target square
    if (!logic.isEnPassantTarget(toRank, toFile)) 
    {
        return false;
    }
    
    // Get the en passant pawn position
    std::pair<int, int> enPassantPawn = logic.getEnPassantPawn();
    int pawnRank = enPassantPawn.first;
    int pawnFile = enPassantPawn.second;
    
    // Verify the en passant pawn exists and is the opponent's pawn
    ChessLogic::Piece targetPawn = logic.getPieceAt(pawnRank, pawnFile);
    if (targetPawn == ChessLogic::Piece::EMPTY) 
    {
        return false;
    }
    
    // Verify it's an opponent's pawn
    ChessLogic::Piece movingPiece = logic.getPieceAt(fromRank, fromFile);
    if (logic.areSameColorPieces(fromRank, fromFile, pawnRank, pawnFile)) 
    {
        return false; // Cannot capture your own pawn
    }
    
    // Verify the capturing pawn is on the correct rank for en passant
    int expectedRank = (movingPiece == ChessLogic::Piece::WHITE_PAWN) ? 4 : 3;
    if (fromRank != expectedRank) 
    {
        return false;
    }
    
    return true; // Valid en passant capture
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
        case ChessLogic::Piece::BLACK_BISHOP: 
        {
            int rankDiff = abs(toRank - fromRank);
            int fileDiff = abs(toFile - fromFile);
            return (rankDiff == fileDiff) && 
                   isPathClearForSlidingPiece(logic, fromRank, fromFile, toRank, toFile);
        }
        
        case ChessLogic::Piece::WHITE_QUEEN:
        case ChessLogic::Piece::BLACK_QUEEN: 
        {
            int rankDiff = abs(toRank - fromRank);
            int fileDiff = abs(toFile - fromFile);
            return ((fromRank == toRank || fromFile == toFile || rankDiff == fileDiff) && 
                    isPathClearForSlidingPiece(logic, fromRank, fromFile, toRank, toFile));
        }
        
        case ChessLogic::Piece::WHITE_KNIGHT:
        case ChessLogic::Piece::BLACK_KNIGHT: 
        {
            int rankDiff = abs(toRank - fromRank);
            int fileDiff = abs(toFile - fromFile);
            return (rankDiff == 2 && fileDiff == 1) || (rankDiff == 1 && fileDiff == 2);
        }
        
        case ChessLogic::Piece::WHITE_KING:
        case ChessLogic::Piece::BLACK_KING: 
        {
            int rankDiff = abs(toRank - fromRank);
            int fileDiff = abs(toFile - fromFile);
            
            // Normal king move (one square in any direction)
            if (rankDiff <= 1 && fileDiff <= 1) 
            {
                return true;
            }
            
            // Castling move (king moves 2 squares horizontally)
            if (rankDiff == 0 && fileDiff == 2) 
            {
                return validateCastling(logic, fromRank, fromFile, toRank, toFile);
            }
            
            return false;
        }
        
        default:
            return false;
    }
}

bool ChessMoveValidator::validatePromotion(
    const ChessLogic& logic, 
    int fromRank, 
    int fromFile, 
    int toRank, 
    int toFile) const 
{
    ChessLogic::Piece piece = logic.getPieceAt(fromRank, fromFile);
    
    // Check if the piece is a pawn
    if (piece != ChessLogic::Piece::WHITE_PAWN && piece != ChessLogic::Piece::BLACK_PAWN) 
    {
        return false; // Not a pawn
    }
    
    // Determine promotion rank based on pawn color
    int promotionRank = (piece == ChessLogic::Piece::WHITE_PAWN) 
                        ? BoardCfg::WHITE_PROMOTION_RANK 
                        : BoardCfg::BLACK_PROMOTION_RANK;
    
    // Check if the pawn is moving to the promotion rank
    if (toRank != promotionRank) 
    {
        return false; // Not moving to promotion rank
    }
    
    // Check if the move is a valid pawn move to the promotion rank
    return validatePawnMove(logic, fromRank, fromFile, toRank, toFile);
}