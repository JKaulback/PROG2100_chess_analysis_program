#include "chess_game_state_analyzer.h"

using StateAnalyzer = ChessGameStateAnalyzer;
namespace BoardCfg = Config::Board;

StateAnalyzer::GameState StateAnalyzer::analyzeGameState(
    const ChessBoard& board,
    const ChessGameState& gameState) {
    // Check 50-Move Rule
    if (isDraw50Moves(board)) 
        return StateAnalyzer::GameState::DRAW_50_MOVES;
    
    // Check for insufficient material
    if (isInsufficientMaterial(board)) 
        return StateAnalyzer::GameState::DRAW_INSUFFICIENT_MATERIAL;
    
    // Check for checkmate
    if (isCheckmate(board, gameState))        
        return (gameState.getCurrentPlayer() == 'w') ?
            StateAnalyzer::GameState::BLACK_WIN :
            StateAnalyzer::GameState::WHITE_WIN;

    // Check for stalemate
    if (isStalemate(board, gameState)) 
        return StateAnalyzer::GameState::STALEMATE;

    // Check for threefold repetition
    if (isThreefoldRepetition(gameState)) 
        return StateAnalyzer::GameState::DRAW_THREEFOLD_REPETITION;

    // Return IN_PROGRESS if no ending conditions are met
    return StateAnalyzer::GameState::IN_PROGRESS;
}

bool StateAnalyzer::isDraw50Moves(const ChessGameState& gameState) const {
    return (gameState.getHalfmoveClock() >= 100); // Check if halfmove clock reaches 50 full moves
}

bool StateAnalyzer::isInsufficientMaterial(const ChessBoard& board) const {
    // Count relevant material (assume kings exist)
    int whiteBishopCount = 0;
    int whiteKnightCount = 0;
    int blackBishopCount = 0;
    int blackKnightCount = 0;
    bool sufficientMaterial = false; // Assume there is insufficient material until discovered otherwise
    // Check each board position
    for (int rank = BoardCfg::MIN_RANK; rank <= BoardCfg::MAX_RANK; rank++) {
        for (int file = BoardCfg::MIN_FILE; file <= BoardCfg::MAX_FILE; file++) {
            char piece = board.getPieceAt(rank, file);
            // If empty, skip
            if (piece == BoardCfg::EMPTY) 
                continue;
            // If a pawn exists, there is sufficient material
            else if (board.isPawn(piece))
                sufficientMaterial = true;
            // Check for white bishops
            else if (piece == 'B') 
                whiteBishopCount++;
            // Check for black bishops
            else if (piece == 'b') 
                blackBishopCount++;
            // Check for white knights
            else if (piece == 'N') 
                whiteKnightCount++;
            // Check for black knights
            else if (piece == 'n') 
                blackKnightCount++;
            // For any other piece except king
            else if (!board.isKing(piece))
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

bool StateAnalyzer::isCheckmate(const ChessBoard& board, const ChessGameState& gameState) const {
    return (isInCheck(board, gameState) && !hasLegalMoves(board, gameState)); // Logic for checkmate
}

bool StateAnalyzer::isStalemate(const ChessBoard& board, const ChessGameState& gameState) const {
    return (!isInCheck(board, gameState) && !hasLegalMoves(board, gameState)); // Logic for stalemate
}

bool StateAnalyzer::isThreefoldRepetition(const ChessGameState& gameState) const
{
    return gameState.hasThreefoldRepetition();
}

// --- HELPERS ---
bool StateAnalyzer::isInCheck(const ChessBoard& board, const ChessGameState& gameState) const {
    char currentPlayer = gameState.getCurrentPlayer();
    std::pair<int, int> kingPosition = board.getKingPosition(currentPlayer);
    char opponent = 
        (currentPlayer == 'w') ?
        'b' :
        'w';

    return validator.isSquareUnderAttack(board, gameState, kingPosition.first, kingPosition.second, opponent);
}

bool StateAnalyzer::hasLegalMoves(const ChessBoard& board, const ChessGameState& gameState) const {
    char currentPlayer = gameState.getCurrentPlayer();

    // Iterate through all squares on the board to find current player's pieces
    for (int srcRank = BoardCfg::MIN_RANK; srcRank <= BoardCfg::MAX_RANK; srcRank++) {
        for (int srcFile = BoardCfg::MIN_FILE; srcFile <= BoardCfg::MAX_FILE; srcFile++){
            char piece = board.getPieceAt(srcRank, srcFile);
            
            // Skip empty squares
            if (piece == BoardCfg::EMPTY) 
                continue;
            
            // Check if this piece belongs to the current player
            bool isCurrentPlayerPiece = 
                (currentPlayer == 'w') ?
                board.isWhitePiece(srcRank, srcFile) :
                !board.isWhitePiece(srcRank, srcFile);
            
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
                        validator.validateMove(board, gameState, ChessMove{srcRank, srcFile, destRank, destFile});
                    
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
