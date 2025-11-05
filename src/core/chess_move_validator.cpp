#include "chess_move_validator.h"
#include "chess_logic.h"
#include <cmath> // For abs() function

// Namespace and type aliases for cleaner code
namespace BoardCfg = Config::Board;

ChessMoveValidator::MoveResult ChessMoveValidator::validateMove(
    const ChessLogic& logic,
    const int srcRank, 
    const int srcFile, 
    const int destRank, 
    const int destFile) const {
    // 1. Check if source and destination are within board boundaries
    if (!logic.isValidSquare(srcRank, srcFile) || 
            !logic.isValidSquare(destRank, destFile)) 
        return MoveResult::INVALID_OUT_OF_BOUNDS;
    
    // 2. Check if moving to the same position
    if (srcRank == destRank && srcFile == destFile) 
        return MoveResult::INVALID_SAME_POSITION;
    
    // 3. Check if there's a piece to move
    ChessLogic::Piece piece = logic.getPieceAt(srcRank, srcFile);
    if (piece == ChessLogic::Piece::EMPTY) 
        return MoveResult::INVALID_NO_PIECE;
    
    // 4. Check if it's the correct player's turn
    ChessLogic::Player currentPlayer = logic.getCurrentPlayer();
    if ((currentPlayer == ChessLogic::Player::WHITE_PLAYER && 
            !logic.isWhitePiece(srcRank, srcFile)) || 
            (currentPlayer == ChessLogic::Player::BLACK_PLAYER && 
            !logic.isBlackPiece(srcRank, srcFile))) 
        return MoveResult::INVALID_WRONG_TURN;
    
    // 5. Check destination square validity
    if (!checkDestinationSquare(logic, srcRank, srcFile, destRank, destFile)) 
        return MoveResult::INVALID_ILLEGAL_MOVE;

    // 6. Validate piece-specific movement rules (includes special move validation internally)
    if (!validatePieceMovement(logic, srcRank, srcFile, destRank, destFile)) 
        return MoveResult::INVALID_ILLEGAL_MOVE;

    // 7. Validate the player's king is not left in check
    if (wouldLeaveKingInCheck(logic, srcRank, srcFile, destRank, destFile)) 
        return MoveResult::INVALID_ILLEGAL_MOVE;

    // 8. Determine the type of valid move for return value
    ChessLogic::Piece movingPiece = logic.getPieceAt(srcRank, srcFile);
    
    // Check for castling (king-specific move)
    if (logic.isKing(movingPiece) && 
            validateCastling(logic, srcRank, srcFile, destRank, destFile)) {
        bool isKingside = (destFile > srcFile);
        return isKingside ? MoveResult::VALID_CASTLE_KINGSIDE : MoveResult::VALID_CASTLE_QUEENSIDE;
    }

    // Check for en passant (pawn-specific move)
    if (logic.isPawn(movingPiece) && 
            validateEnPassant(logic, srcRank, srcFile, destRank, destFile)) 
        return MoveResult::VALID_EN_PASSANT;
    
    // Check for promotion (pawn-specific move)
    if (logic.isPawn(movingPiece) && 
            validatePromotion(logic, srcRank, srcFile, destRank, destFile)) 
        return MoveResult::VALID_PROMOTION;

    // If we get here, the move is a standard valid move
    return MoveResult::VALID;
}

bool ChessMoveValidator::validatePieceMovement(
    const ChessLogic& logic, 
    const int srcRank, 
    const int srcFile, 
    const int destRank, 
    const int destFile) const {
    ChessLogic::Piece piece = logic.getPieceAt(srcRank, srcFile);
    
    // Special case for pawns - they have unique movement rules
    if (logic.isPawn(piece)) 
        return validatePawnMove(logic, srcRank, srcFile, destRank, destFile);
    
    // For all other pieces, use the unified basic movement validation
    return validateBasicPieceMovement(logic, piece, srcRank, srcFile, destRank, destFile);
}

bool ChessMoveValidator::checkDestinationSquare(
    const ChessLogic& logic, 
    const int srcRank, 
    const int srcFile, 
    const int destRank, 
    const int destFile) const {
    // True if the destination square is empty or contains an opposing piece
    return (logic.isSquareEmpty(destRank, destFile) || 
        !logic.areSameColorPieces(srcRank, srcFile, destRank, destFile));
}

bool ChessMoveValidator::isPathClearForSlidingPiece(
    const ChessLogic& logic, 
    int srcRank, 
    int srcFile, 
    int destRank, 
    int destFile) const {
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
        if (!logic.isSquareEmpty(currentRank, currentFile))
            return false;
        // Go to next square
        currentRank += rankStep;
        currentFile += fileStep;
    }

    return true; // Path is clear
}

bool ChessMoveValidator::validatePawnMove(
    const ChessLogic& logic, 
    const int srcRank, 
    const int srcFile, 
    const int destRank, 
    const int destFile) const {
    // Setup variables for checking
    ChessLogic::Piece piece = logic.getPieceAt(srcRank, srcFile);
    int direction = 
        (piece == ChessLogic::Piece::WHITE_PAWN) ? 
        BoardCfg::WHITE_PAWN_DIRECTION : 
        BoardCfg::BLACK_PAWN_DIRECTION;
    int startRank = 
        (piece == ChessLogic::Piece::WHITE_PAWN) ? 
        BoardCfg::WHITE_PAWN_START_RANK : 
        BoardCfg::BLACK_PAWN_START_RANK;

    // Standard move forward
    if (destFile == srcFile) {    // One square forward
        if (destRank == srcRank + direction && 
                logic.isSquareEmpty(destRank, destFile)) 
            return true;
        // Two squares forward src starting position
        if (srcRank == startRank && destRank == srcRank + 2 * direction &&
                logic.isSquareEmpty(srcRank + direction, destFile) &&
                logic.isSquareEmpty(destRank, destFile)) 
            return true;
    }
    // Capture move (including en passant, diagonal check)
    else if (abs(destFile - srcFile) == 1 && destRank == srcRank + direction) { 
        // Regular capture
        if (!logic.isSquareEmpty(destRank, destFile) &&
                !logic.areSameColorPieces(srcRank, srcFile, destRank, destFile)) 
            return true;
        // En passant capture
        else if (logic.isSquareEmpty(destRank, destFile) && 
                logic.isEnPassantTarget(destRank, destFile)) 
            return validateEnPassant(logic, srcRank, srcFile, destRank, destFile);
    }
    // Invalid pawn move
    return false;
}

bool ChessMoveValidator::wouldLeaveKingInCheck(
    const ChessLogic& logic, 
    const int srcRank, 
    const int srcFile, 
    const int destRank, 
    const int destFile) const {
    // Get player and opponent
    ChessLogic::Player currentPlayer = logic.getCurrentPlayer();
    ChessLogic::Player opponent = 
        (currentPlayer == ChessLogic::Player::WHITE_PLAYER) ? 
        ChessLogic::Player::BLACK_PLAYER : 
        ChessLogic::Player::WHITE_PLAYER;
    
    // Create a copy to test the move (efficient for standard chess board)
    ChessLogic testLogic = logic;
    
    // Make the temporary move
    testLogic.makeTemporaryMove(srcRank, srcFile, destRank, destFile);
    
    // Find the king position after the move
    std::pair<int, int> kingPos = testLogic.getKingPosition(currentPlayer);
    
    // Check if the king is under attack
    return isSquareUnderAttack(testLogic, kingPos.first, kingPos.second, opponent);
}

bool ChessMoveValidator::isSquareUnderAttack(
    const ChessLogic& logic, 
    const int defRank, 
    const int defFile, 
    const ChessLogic::Player attackingPlayer) const {
    // Check all squares on the board for pieces belonging to the attacking player
    for (int atkRank = BoardCfg::MIN_RANK; atkRank <= BoardCfg::MAX_RANK; atkRank++) {
        for (int atkFile = BoardCfg::MIN_FILE; atkFile <= BoardCfg::MAX_FILE; atkFile++) {
            ChessLogic::Piece attackingPiece = logic.getPieceAt(atkRank, atkFile);
            
            // Skip empty squares
            if (attackingPiece == ChessLogic::Piece::EMPTY) 
                continue;
            
            // Check if this piece belongs to the attacking player
            bool isAttackersPiece = 
                (attackingPlayer == ChessLogic::Player::WHITE_PLAYER) ?
                logic.isWhitePiece(atkRank, atkFile) :
                logic.isBlackPiece(atkRank, atkFile);
            
            if (!isAttackersPiece) 
                continue;
            
            // Check if this piece can attack the target square
            // For pawns, special logic since they attack differently than they move
            if (logic.isPawn(attackingPiece)) {
                int direction =
                    (attackingPiece == ChessLogic::Piece::WHITE_PAWN) ? 
                    BoardCfg::WHITE_PAWN_DIRECTION : 
                    BoardCfg::BLACK_PAWN_DIRECTION;
                
                if (defRank == atkRank + direction && abs(defFile - atkFile) == 1) 
                    return true; // Pawn can attack diagonally
            } else {
                // For other pieces, use existing movement validation
                if (validateBasicPieceMovement(logic, attackingPiece, atkRank, atkFile, defRank, defFile))
                    return true;
            }
        }
    }
    return false; // No piece can attack this square
}

bool ChessMoveValidator::validateCastling(
    const ChessLogic& logic,
    const int srcRank,
    const int srcFile,
    const int destRank,
    const int destFile) const {
    // Get the player attempting to castle
    ChessLogic::Player currentPlayer = logic.getCurrentPlayer();
    
    // Check if this is a king making a castling move
    ChessLogic::Piece piece = logic.getPieceAt(srcRank, srcFile);
    if (piece != ChessLogic::Piece::WHITE_KING && piece != ChessLogic::Piece::BLACK_KING) 
        return false; // Not a king move

    // Check if the move is a potential castling move (king moves 2 squares horizontally)
    int rankDiff = abs(destRank - srcRank);
    int fileDiff = abs(destFile - srcFile);
    if (rankDiff != 0 || fileDiff != 2) 
        return false; // Not a castling move pattern
    
    // Determine if it's kingside or queenside
    bool isKingside = (destFile > srcFile);
    
    return canCastle(logic, currentPlayer, isKingside);
}

bool ChessMoveValidator::canCastle(
    const ChessLogic& logic, 
    const ChessLogic::Player player, 
    const bool isKingside) const {
    // Check basic castling rights
    if (isKingside && !logic.canCastleKingside(player)) 
        return false;
    if (!isKingside && !logic.canCastleQueenside(player)) 
        return false;
    
    // Get king and rook positions
    int kingRank = 
        (player == ChessLogic::Player::WHITE_PLAYER) ? 
        BoardCfg::WHITE_BACK_RANK : 
        BoardCfg::BLACK_BACK_RANK;
    int kingFile = BoardCfg::KING_FILE;
    int rookFile = isKingside ? 
        BoardCfg::KINGSIDE_ROOK_FILE : 
        BoardCfg::QUEENSIDE_ROOK_FILE;
    
    // Check if king is currently in check
    ChessLogic::Player opponent = 
        (player == ChessLogic::Player::WHITE_PLAYER) ? 
        ChessLogic::Player::BLACK_PLAYER :
        ChessLogic::Player::WHITE_PLAYER;
    
    if (isSquareUnderAttack(logic, kingRank, kingFile, opponent)) 
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
        if (!logic.isSquareEmpty(kingRank, file)) 
            return false; // Path is blocked
    }
    
    // Check if king passes through check
    int direction = isKingside ? 1 : -1;
    for (int i = 1; i <= 2; ++i) {
        int testFile = kingFile + (i * direction);
        if (isSquareUnderAttack(logic, kingRank, testFile, opponent)) 
            return false; // Cannot castle through check
    }
    
    return true; // All castling conditions met
}

bool ChessMoveValidator::validateEnPassant(
    const ChessLogic& logic,
    const int srcRank,
    const int srcFile,
    const int destRank,
    const int destFile) const {
    // Check if en passant is available
    if (!logic.isEnPassantAvailable()) 
        return false;
    
    // Check if the destination is the en passant target square
    if (!logic.isEnPassantTarget(destRank, destFile)) 
        return false;
    
    // Get the en passant pawn position
    std::pair<int, int> enPassantPawn = logic.getEnPassantPawn();
    int pawnRank = enPassantPawn.first;
    int pawnFile = enPassantPawn.second;
    
    // Verify the en passant pawn exists and is the opponent's pawn
    ChessLogic::Piece targetPawn = logic.getPieceAt(pawnRank, pawnFile);
    if (targetPawn == ChessLogic::Piece::EMPTY) 
        return false;
    
    // Verify it's an opponent's pawn
    ChessLogic::Piece movingPiece = logic.getPieceAt(srcRank, srcFile);
    if (logic.areSameColorPieces(srcRank, srcFile, pawnRank, pawnFile)) 
        return false; // Cannot capture your own pawn
    
    // Verify the capturing pawn is on the correct rank for en passant
    int expectedRank = 
        (movingPiece == ChessLogic::Piece::WHITE_PAWN) ? 
        BoardCfg::WHITE_EN_PASSANT_CAPTURE_RANK : 
        BoardCfg::BLACK_EN_PASSANT_CAPTURE_RANK;
    if (srcRank != expectedRank) 
        return false;
    
    return true; // Valid en passant capture
}

bool ChessMoveValidator::validateBasicPieceMovement(
    const ChessLogic& logic, 
    const ChessLogic::Piece piece, 
    const int srcRank, 
    const int srcFile, 
    const int destRank, 
    const int destFile) const {
    // This validates piece movement without checking destination square ownership
    // Used for attack detection where we want to know if a piece CAN reach a square
    switch (piece) {
        case ChessLogic::Piece::WHITE_ROOK:
        case ChessLogic::Piece::BLACK_ROOK:
            return (srcRank == destRank || srcFile == destFile) && // Horizontal and vertical
                isPathClearForSlidingPiece(logic, srcRank, srcFile, destRank, destFile); // Sliding piece
        
        case ChessLogic::Piece::WHITE_BISHOP:
        case ChessLogic::Piece::BLACK_BISHOP:
            return (abs(destRank - srcRank) == abs(destFile - srcFile)) && // Diagonal 
                isPathClearForSlidingPiece(logic, srcRank, srcFile, destRank, destFile); // Sliding piece
        
        case ChessLogic::Piece::WHITE_QUEEN:
        case ChessLogic::Piece::BLACK_QUEEN: 
            return ((srcRank == destRank || srcFile == destFile || // Horizontal and vertical
                abs(destRank - srcRank) == abs(destFile - srcFile)) && // Diagonal
                isPathClearForSlidingPiece(logic, srcRank, srcFile, destRank, destFile)); // Sliding piece
        
        case ChessLogic::Piece::WHITE_KNIGHT:
        case ChessLogic::Piece::BLACK_KNIGHT: 
            return (abs(destRank - srcRank) == 2 && abs(destFile - srcFile) == 1) ||  // Vertical L
                (abs(destRank - srcRank) == 1 && abs(destFile - srcFile) == 2); // Horizontal L
        
        case ChessLogic::Piece::WHITE_KING:
        case ChessLogic::Piece::BLACK_KING: { 
            // Normal king move (one square in any direction)
            if (abs(destRank - srcRank) <= 1 && abs(destFile - srcFile) <= 1) 
                return true;            
            // Castling move (king moves 2 squares horizontally)
            if (abs(destRank - srcRank) == 0 && abs(destFile - srcFile) == 2) 
                return validateCastling(logic, srcRank, srcFile, destRank, destFile);
            
            return false;
        }
        default:
            return false;
    }
}

bool ChessMoveValidator::validatePromotion(
    const ChessLogic& logic, 
    const int srcRank, 
    const int srcFile, 
    const int destRank, 
    const int destFile) const {
        // Get the piece being promoted
    ChessLogic::Piece piece = logic.getPieceAt(srcRank, srcFile);
    
    // Determine promotion rank based on pawn color
    int promotionRank = 
        (piece == ChessLogic::Piece::WHITE_PAWN) ? 
        BoardCfg::WHITE_PROMOTES_AT_RANK : 
        BoardCfg::BLACK_PROMOTES_AT_RANK;
    
    // Check if the pawn is moving to the promotion rank
    if (destRank != promotionRank) 
        return false; // Not moving to promotion rank
    
    // Check if the move is a valid pawn move to the promotion rank
    return validatePawnMove(logic, srcRank, srcFile, destRank, destFile);
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