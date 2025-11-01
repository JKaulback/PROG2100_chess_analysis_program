#include "chess_game_state_analyzer.h"

using StateAnalyzer = ChessGameStateAnalyzer;
namespace BoardCfg = Config::Board;

StateAnalyzer::GameState StateAnalyzer::analyzeGameState(const ChessLogic& logic)
{
    // 50-Move Rule
    if (isDraw50Moves(logic)) return StateAnalyzer::GameState::DRAW_50_MOVES;
    
    // Insufficient material
    if (isInsufficientMaterial(logic)) return StateAnalyzer::GameState::DRAW_INSUFFICIENT_MATERIAL;
    
    // For now, return IN_PROGRESS if no ending conditions are met
    return StateAnalyzer::GameState::IN_PROGRESS;
}

bool StateAnalyzer::isDraw50Moves(const ChessLogic& logic)
{
    return (logic.getHalfmoveClock() >= 100);
}

bool StateAnalyzer::isInsufficientMaterial(const ChessLogic& logic)
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