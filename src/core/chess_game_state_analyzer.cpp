#include "chess_game_state_analyzer.h"

using StateAnalyzer = ChessGameStateAnalyzer;

StateAnalyzer::GameState StateAnalyzer::analyzeGameState(const ChessLogic& logic)
{
    // 50-Move Rule
    if (isDraw50Moves(logic)) return StateAnalyzer::GameState::DRAW_50_MOVES;
    
    // TODO: Add other game state checks here (checkmate, stalemate, etc.)
    
    // For now, return IN_PROGRESS if no ending conditions are met
    return StateAnalyzer::GameState::IN_PROGRESS;
}

bool StateAnalyzer::isDraw50Moves(const ChessLogic& logic)
{
    return (logic.getHalfmoveClock() >= 100);
}