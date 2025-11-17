#include "chess_game_state_analyzer.h"

using StateAnalyzer = ChessGameStateAnalyzer;
namespace BoardCfg = Config::Board;

StateAnalyzer::GameState StateAnalyzer::analyzeGameState(const ChessLogic& logic) {
    // Check 50-Move Rule
    if (isDraw50Moves(logic)) 
        return StateAnalyzer::GameState::DRAW_50_MOVES;
    
    // Check for insufficient material
    if (isInsufficientMaterial(logic)) 
        return StateAnalyzer::GameState::DRAW_INSUFFICIENT_MATERIAL;
    
    // Check for checkmate
    if (isCheckmate(logic))        
        return (logic.getCurrentPlayer() == ChessLogic::Player::WHITE_PLAYER) ?
            StateAnalyzer::GameState::BLACK_WIN :
            StateAnalyzer::GameState::WHITE_WIN;

    // Check for stalemate
    if (isStalemate(logic)) 
        return StateAnalyzer::GameState::STALEMATE;

    // Check for threefold repetition
    if (isThreefoldRepetition(logic)) 
        return StateAnalyzer::GameState::DRAW_THREEFOLD_REPETITION;

    // Return IN_PROGRESS if no ending conditions are met
    return StateAnalyzer::GameState::IN_PROGRESS;
}

bool StateAnalyzer::isDraw50Moves(const ChessLogic& logic) const {
    return (logic.getHalfmoveClock() >= 100); // Check if halfmove clock reaches 50 full moves
}

bool StateAnalyzer::isInsufficientMaterial(const ChessLogic& logic) const {
    // Count relevant material (assume kings exist)
    int whiteBishopCount = 0;
    int whiteKnightCount = 0;
    int blackBishopCount = 0;
    int blackKnightCount = 0;
    bool sufficientMaterial = false; // Assume there is insufficient material until discovered otherwise
    // Check each board position
    for (int rank = BoardCfg::MIN_RANK; rank <= BoardCfg::MAX_RANK; rank++) {
        for (int file = BoardCfg::MIN_FILE; file <= BoardCfg::MAX_FILE; file++) {
            ChessLogic::Piece piece = logic.getPieceAt(rank, file);
            // If empty, skip
            if (piece == ChessLogic::Piece::EMPTY) 
                continue;
            // If a pawn exists, there is sufficient material
            else if (piece == ChessLogic::Piece::BLACK_PAWN || 
                    piece == ChessLogic::Piece::WHITE_PAWN)
                sufficientMaterial = true;
            // Check for white bishops
            else if (piece == ChessLogic::Piece::WHITE_BISHOP) 
                whiteBishopCount++;
            // Check for black bishops
            else if (piece == ChessLogic::Piece::BLACK_BISHOP) 
                blackBishopCount++;
            // Check for white knights
            else if (piece == ChessLogic::Piece::WHITE_KNIGHT) 
                whiteKnightCount++;
            // Check for black knights
            else if (piece == ChessLogic::Piece::BLACK_KNIGHT) 
                blackKnightCount++;
            // For any other piece except king
            else if (piece != ChessLogic::Piece::WHITE_KING && piece != ChessLogic::Piece::BLACK_KING)
                sufficientMaterial = true;
            // Check if knight and bishop exist on one side
            if ((whiteBishopCount && whiteKnightCount) || (blackBishopCount && blackKnightCount)) {
                sufficientMaterial = true;
            }
            // Check for multiples
            else if (whiteBishopCount > 1 || whiteKnightCount > 1 || blackBishopCount > 1 || blackKnightCount > 1) {
                sufficientMaterial = true;
            }
            // Check if this cell found sufficient material
            if (sufficientMaterial)
                break;
        }
        // Check if the file found sufficient material
        if (sufficientMaterial) 
            break;
    }
    return !sufficientMaterial;
}

bool StateAnalyzer::isCheckmate(const ChessLogic& logic) const {
    return (isInCheck(logic) && !hasLegalMoves(logic)); // Logic for checkmate
}

bool StateAnalyzer::isStalemate(const ChessLogic& logic) const {
    return (!isInCheck(logic) && !hasLegalMoves(logic)); // Logic for stalemate
}

bool StateAnalyzer::isThreefoldRepetition(const ChessLogic& logic) const
{
    return logic.hasThreefoldRepetition();
}

// --- HELPERS ---
bool StateAnalyzer::isInCheck(const ChessLogic& logic) const {
    ChessLogic::Player currentPlayer = logic.getCurrentPlayer();
    std::pair<int, int> kingPosition = logic.getKingPosition(currentPlayer);
    ChessLogic::Player opponent = 
        (currentPlayer == ChessLogic::Player::WHITE_PLAYER) ?
        ChessLogic::Player::BLACK_PLAYER :
        ChessLogic::Player::WHITE_PLAYER;

    return validator.isSquareUnderAttack(logic, kingPosition.first, kingPosition.second, opponent);
}

bool StateAnalyzer::hasLegalMoves(const ChessLogic& logic) const {
    ChessLogic::Player currentPlayer = logic.getCurrentPlayer();
    
    // Iterate through all squares on the board to find current player's pieces
    for (int srcRank = BoardCfg::MIN_RANK; srcRank <= BoardCfg::MAX_RANK; srcRank++) {
        for (int srcFile = BoardCfg::MIN_FILE; srcFile <= BoardCfg::MAX_FILE; srcFile++){
            ChessLogic::Piece piece = logic.getPieceAt(srcRank, srcFile);
            
            // Skip empty squares
            if (piece == ChessLogic::Piece::EMPTY) 
                continue;
            
            // Check if this piece belongs to the current player
            bool isCurrentPlayerPiece = 
                (currentPlayer == ChessLogic::Player::WHITE_PLAYER) ?
                logic.isWhitePiece(srcRank, srcFile) :
                logic.isBlackPiece(srcRank, srcFile);
            
            if (!isCurrentPlayerPiece) 
                continue;
            
            // Try all possible destination squares for this piece
            for (int destRank = BoardCfg::MIN_RANK; destRank <= BoardCfg::MAX_RANK; destRank++) {
                for (int destFile = BoardCfg::MIN_FILE; destFile <= BoardCfg::MAX_FILE; destFile++){
                    // Skip moving to the same square
                    if (srcRank == destRank && srcFile == destFile) 
                        continue;
                    
                    // Test if this move is valid
                    ChessMoveValidator::MoveResult result = 
                        validator.validateMove(logic, ChessMove{srcRank, srcFile, destRank, destFile});
                    
                    // If we find any legal move, the player has legal moves
                    if (validator.isValidMoveResult(result))
                        return true; // Found a legal move
                }
            }
        }
    }
    // No legal moves found
    return false;
}
