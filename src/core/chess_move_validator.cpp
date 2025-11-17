#include "chess_move_validator.h"
#include <cmath> // For abs() function

// Namespace and type aliases for cleaner code
namespace BoardCfg = Config::Board;

ChessMoveValidator::MoveResult ChessMoveValidator::validateMove(
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
        return MoveResult::INVALID_OUT_OF_BOUNDS;
    
    // 2. Check if moving to the same position
    if (srcRank == destRank && srcFile == destFile) 
        return MoveResult::INVALID_SAME_POSITION;
    
    // 3. Check if there's a piece to move
    char piece = board.getPieceAt(srcRank, srcFile);
    if (piece == BoardCfg::EMPTY) 
        return MoveResult::INVALID_NO_PIECE;
    
    // 4. Check if it's the correct player's turn
    char currentPlayer = gameState.getCurrentPlayer();
    if ((currentPlayer == 'w' && 
            !board.isWhitePiece(srcRank, srcFile)) || 
            (currentPlayer == 'b' && 
            board.isWhitePiece(srcRank, srcFile))) 
        return MoveResult::INVALID_WRONG_TURN;
    
    // 5. Check destination square validity
    if (!checkDestinationSquare(board, move)) 
        return MoveResult::INVALID_ILLEGAL_MOVE;

    // 6. Validate piece-specific movement rules (includes special move validation internally)
    if (!validatePieceMovement(board, gameState, move)) 
        return MoveResult::INVALID_ILLEGAL_MOVE;

    // 7. Validate the player's king is not left in check
    if (wouldLeaveKingInCheck(board, gameState, move)) 
        return MoveResult::INVALID_ILLEGAL_MOVE;

    // 8. Determine the type of valid move for return value
    char movingPiece = board.getPieceAt(srcRank, srcFile);
    
    // Check for castling (king-specific move)
    if (board.isKing(movingPiece) && 
            validateCastling(board, gameState, move)) {
        bool isKingside = (destFile > srcFile);
        return isKingside ? MoveResult::VALID_CASTLE_KINGSIDE : MoveResult::VALID_CASTLE_QUEENSIDE;
    }

    // Check for en passant (pawn-specific move)
    if (board.isPawn(movingPiece) && 
            validateEnPassant(board, gameState, move)) 
        return MoveResult::VALID_EN_PASSANT;
    
    // Check for promotion (pawn-specific move)
    if (board.isPawn(movingPiece) && 
            validatePromotion(board, gameState, move)) 
        return MoveResult::VALID_PROMOTION;

    // If we get here, the move is a standard valid move
    return MoveResult::VALID;
}

bool ChessMoveValidator::validatePieceMovement(
    const ChessBoard& board, 
    const ChessGameState& gameState,
    const ChessMove& move) const {
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destRank = move.getDestRank();
    int destFile = move.getDestFile();
    char piece = board.getPieceAt(srcRank, srcFile);
    
    // Special case for pawns - they have unique movement rules
    if (board.isPawn(piece)) 
        return validatePawnMove(board, gameState, move);
    
    // For all other pieces, use the unified basic movement validation
    return validateBasicPieceMovement(board, gameState, piece, move);
}

bool ChessMoveValidator::checkDestinationSquare(
    const ChessBoard& board, 
    const ChessMove& move) const {
        
    // True if the destination square is empty or contains an opposing piece
    return (board.isSquareEmpty(move.getDestRank(), move.getDestFile()) || 
        !board.areSameColorPieces(move));
}

bool ChessMoveValidator::isPathClearForSlidingPiece(
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

bool ChessMoveValidator::validatePawnMove(
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
    if (destFile == srcFile) {    // One square forward
        if (destRank == srcRank + direction && 
                board.isSquareEmpty(destRank, destFile)) 
            return true;
        // Two squares forward src starting position
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
            return validateEnPassant(board, gameState, move);
    }
    // Invalid pawn move
    return false;
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
                if (validateBasicPieceMovement(board, gameState, attackingPiece, ChessMove{atkRank, atkFile, defRank, defFile}))
                    return true;
            }
        }
    }
    return false; // No piece can attack this square
}

bool ChessMoveValidator::validateCastling(
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

bool ChessMoveValidator::canCastle(
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
    
    if (isSquareUnderAttack(board, gameState, kingRank, kingFile, opponent)) 
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
        if (isSquareUnderAttack(board, gameState, kingRank, testFile, opponent)) 
            return false; // Cannot castle through check
    }
    
    return true; // All castling conditions met
}

bool ChessMoveValidator::validateEnPassant(
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

bool ChessMoveValidator::validateBasicPieceMovement(
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
            // Castling move (king moves 2 squares horizontally)
            if (abs(destRank - srcRank) == 0 && abs(destFile - srcFile) == 2) 
                return validateCastling(board, gameState, move);
            
            return false;
        }
        default:
            return false;
    }
}

bool ChessMoveValidator::validatePromotion(
    const ChessBoard& board, 
    const ChessGameState& gameState,
    const ChessMove& move) const {
        
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destRank = move.getDestRank();
    int destFile = move.getDestFile();
        // Get the piece being promoted
    char piece = board.getPieceAt(srcRank, srcFile);
    
    // Determine promotion rank based on pawn color
    int promotionRank = 
        (piece == 'P') ? 
        BoardCfg::WHITE_PROMOTES_AT_RANK : 
        BoardCfg::BLACK_PROMOTES_AT_RANK;
    
    // Check if the pawn is moving to the promotion rank
    if (destRank != promotionRank) 
        return false; // Not moving to promotion rank
    
    // Check if the move is a valid pawn move to the promotion rank
    return validatePawnMove(board, gameState, move);
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