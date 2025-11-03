#include "chess_game_state_analyzer.h"

using StateAnalyzer = ChessGameStateAnalyzer;
namespace BoardCfg = Config::Board;

StateAnalyzer::GameState StateAnalyzer::analyzeGameState(const ChessLogic& logic)
{
    // 50-Move Rule
    if (isDraw50Moves(logic)) return StateAnalyzer::GameState::DRAW_50_MOVES;
    
    // Insufficient material
    if (isInsufficientMaterial(logic)) return StateAnalyzer::GameState::DRAW_INSUFFICIENT_MATERIAL;
    
    // Checkmate
    if (isCheckmate(logic)) 
    {
        if (logic.getCurrentPlayer() == ChessLogic::Player::WHITE_PLAYER)
        {
            return StateAnalyzer::GameState::BLACK_WIN;
        }
        return StateAnalyzer::GameState::WHITE_WIN;
    }

    // Stalemate
    if (isStalemate(logic)) return StateAnalyzer::GameState::STALEMATE;

    // For now, return IN_PROGRESS if no ending conditions are met
    return StateAnalyzer::GameState::IN_PROGRESS;
}

bool StateAnalyzer::isDraw50Moves(const ChessLogic& logic) const
{
    return (logic.getHalfmoveClock() >= 100);
}

bool StateAnalyzer::isInsufficientMaterial(const ChessLogic& logic) const
{
    int whiteBishopCount = 0;
    int whiteKnightCount = 0;
    int blackBishopCount = 0;
    int blackKnightCount = 0;
    bool insufficientMaterial = true;
    for (int rank = BoardCfg::MIN_RANK; rank <= BoardCfg::MAX_RANK; rank++)
    {
        for (int file = BoardCfg::MIN_FILE; file <= BoardCfg::MAX_FILE; file++)
        {
            ChessLogic::Piece piece = logic.getPieceAt(rank, file);
            // If empty, skip
            if (piece == ChessLogic::Piece::EMPTY)
            {
                continue;
            }
            // Check if a pawn exists
            else if (piece == ChessLogic::Piece::BLACK_PAWN ||
                piece == ChessLogic::Piece::WHITE_PAWN)
            {
                insufficientMaterial = false;
                break;
            }
            // Check for white bishops
            else if (piece == ChessLogic::Piece::WHITE_BISHOP)
            {
                whiteBishopCount++;
            }
            // Check for black bishops
            else if (piece == ChessLogic::Piece::BLACK_BISHOP)
            {
                blackBishopCount++;
            }
            // Check for white knights
            else if (piece == ChessLogic::Piece::WHITE_KNIGHT)
            {
                whiteKnightCount++;
            }
            // Check for black knights
            else if (piece == ChessLogic::Piece::BLACK_KNIGHT)
            {
                blackKnightCount++;
            }
            // For any other piece except king
            else if (piece != ChessLogic::Piece::WHITE_KING &&
                piece != ChessLogic::Piece::BLACK_KING)
            {
                insufficientMaterial = false;
                break;
            }

            // Check if knight and bishop exist on one side
            if ((whiteBishopCount && whiteKnightCount) ||
                (blackBishopCount && blackKnightCount)) 
            {
                insufficientMaterial = false;
                break;
            }
            // Check for multiples
            else if (whiteBishopCount > 1 || whiteKnightCount > 1 ||
                    blackBishopCount > 1 || blackKnightCount > 1)
            {
                insufficientMaterial = false;
                break;
            }
        }
        if (!insufficientMaterial) break;
    }

    return insufficientMaterial;
}

bool StateAnalyzer::isCheckmate(const ChessLogic& logic) const
{
    return (isInCheck(logic) && !hasLegalMoves(logic));
}

bool StateAnalyzer::isStalemate(const ChessLogic& logic) const
{
    return (!isInCheck(logic) && !hasLegalMoves(logic));
}

// --- HELPERS ---
bool StateAnalyzer::isInCheck(const ChessLogic& logic) const
{
    ChessMoveValidator validator;
    
    ChessLogic::Player currentPlayer = logic.getCurrentPlayer();
    std::pair<int, int> kingPosition = logic.getKingPosition(currentPlayer);
    ChessLogic::Player opponent = (currentPlayer == ChessLogic::Player::WHITE_PLAYER) ?
                                    ChessLogic::Player::BLACK_PLAYER :
                                    ChessLogic::Player::WHITE_PLAYER;

    return validator.isSquareUnderAttack(logic, kingPosition.first, kingPosition.second, opponent);
}

bool StateAnalyzer::hasLegalMoves(const ChessLogic& logic) const
{
    ChessMoveValidator validator;
    ChessLogic::Player currentPlayer = logic.getCurrentPlayer();
    
    // Iterate through all squares on the board to find current player's pieces
    for (int fromRank = BoardCfg::MIN_RANK; fromRank <= BoardCfg::MAX_RANK; fromRank++)
    {
        for (int fromFile = BoardCfg::MIN_FILE; fromFile <= BoardCfg::MAX_FILE; fromFile++)
        {
            ChessLogic::Piece piece = logic.getPieceAt(fromRank, fromFile);
            
            // Skip empty squares
            if (piece == ChessLogic::Piece::EMPTY) continue;
            
            // Check if this piece belongs to the current player
            bool isCurrentPlayerPiece = false;
            if (currentPlayer == ChessLogic::Player::WHITE_PLAYER)
            {
                isCurrentPlayerPiece = logic.isWhitePiece(fromRank, fromFile);
            }
            else
            {
                isCurrentPlayerPiece = logic.isBlackPiece(fromRank, fromFile);
            }
            
            if (!isCurrentPlayerPiece) continue;
            
            // Try all possible destination squares for this piece
            for (int toRank = BoardCfg::MIN_RANK; toRank <= BoardCfg::MAX_RANK; toRank++)
            {
                for (int toFile = BoardCfg::MIN_FILE; toFile <= BoardCfg::MAX_FILE; toFile++)
                {
                    // Skip moving to the same square
                    if (fromRank == toRank && fromFile == toFile) continue;
                    
                    // Test if this move is valid
                    ChessMoveValidator::MoveResult result = validator.validateMove(
                        logic, fromRank, fromFile, toRank, toFile
                    );
                    
                    // If we find any legal move, the player has legal moves
                    if (validator.isValidMoveResult(result))
                    {
                        return true; // Found a legal move
                    }
                }
            }
        }
    }
    // No legal moves found
    return false;
}
