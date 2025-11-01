#include "chess_analysis_program.h"

namespace GOCfg = Config::GameOver;

ChessAnalysisProgram::ChessAnalysisProgram() : 
    logic{}, gui{*this}, inputHandler{}, moveValidator{}, gameStateAnalyzer{},
    currentGameState{GameState::IN_PROGRESS}
{}

ChessAnalysisProgram::~ChessAnalysisProgram() 
{}

void ChessAnalysisProgram::run() 
{

    // Main loop
    while (!WindowShouldClose()) 
    { // Detect window close button or ESC key
        inputHandler.handleInput(*this, gui); // Input handler processes input through controller
        gui.draw(); // GUI only renders
    }

}

// Delegate methods to logic
ChessLogic::Piece ChessAnalysisProgram::getPieceAt(int rank, int file) const 
{
    return logic.getPieceAt(rank, file);
}

std::string ChessAnalysisProgram::pieceToString(ChessLogic::Piece piece) const 
{
    return logic.pieceToString(piece);
}

ChessLogic::Player ChessAnalysisProgram::getCurrentPlayer() const 
{
    return logic.getCurrentPlayer();
}


std::vector<ChessLogic::Piece> ChessAnalysisProgram::getCapturedPieces() const
{
    return logic.getCapturedPieces();
}

ChessLogic::Player ChessAnalysisProgram::getPieceOwner(const ChessLogic::Piece piece) const
{
    return logic.getPieceOwner(piece);
}

// Delegate methods to input handler
bool ChessAnalysisProgram::getIsDragging() const 
{
    return inputHandler.getIsDragging();
}

int ChessAnalysisProgram::getDraggedPieceRank() const 
{
    return inputHandler.getDraggedPieceRank();
}

int ChessAnalysisProgram::getDraggedPieceFile() const 
{
    return inputHandler.getDraggedPieceFile();
}

Vector2 ChessAnalysisProgram::getDragOffset() const 
{
    return inputHandler.getDragOffset();
}

ChessLogic::Piece ChessAnalysisProgram::getDraggedPiece() const 
{
    return inputHandler.getDraggedPiece();
}

// Move validation and execution methods (Controller coordination)
bool ChessAnalysisProgram::attemptMove(int srcRank, int srcFile, int destRank, int destFile) 
{
    bool isSuccessful = false;
    // 1. Validate the move using the validator
    MoveResult validationResult = moveValidator.validateMove(logic, srcRank, srcFile, destRank, destFile);
    
    // 2. If valid move, execute the move and switch turns
    if (isValidMoveResult(validationResult)) 
    {
        // Handle special moves
        if (validationResult == MoveResult::VALID_CASTLE_KINGSIDE || 
            validationResult == MoveResult::VALID_CASTLE_QUEENSIDE) 
        {
            logic.executeCastling(srcRank, srcFile, destRank, destFile);
        } 
        else if (validationResult == MoveResult::VALID_EN_PASSANT) 
        {
            logic.executeEnPassant(srcRank, srcFile, destRank, destFile);
        } 
        else if (validationResult == MoveResult::VALID_PROMOTION)
        {
            logic.executePromotion(srcRank, srcFile, destRank, destFile);
        }
        else
        {
            logic.executeMove(srcRank, srcFile, destRank, destFile);
        }
        
        logic.switchTurn(); // Switch to the other player
        
        // After successful move, analyze the new game state
        currentGameState = gameStateAnalyzer.analyzeGameState(logic);
        isSuccessful = true;
    }

    // 4. Move was invalid - return false
    return isSuccessful;
}

GameState ChessAnalysisProgram::getGameState() const
{
    return currentGameState;
}

bool ChessAnalysisProgram::isGameOver() const
{
    return currentGameState != GameState::IN_PROGRESS;
}

bool ChessAnalysisProgram::isValidMoveResult(MoveResult result) const 
{
    return result == MoveResult::VALID || 
           result == MoveResult::VALID_CASTLE_KINGSIDE || 
           result == MoveResult::VALID_CASTLE_QUEENSIDE ||
           result == MoveResult::VALID_EN_PASSANT ||
           result == MoveResult::VALID_PROMOTION;
}

std::string ChessAnalysisProgram::getGameOverString() const
{
    std::string gameOverText;   
    switch (currentGameState) {
        case GameState::WHITE_WIN:
            gameOverText = GOCfg::WHITE_WIN_STRING;
            break;
        case GameState::BLACK_WIN:
            gameOverText = GOCfg::BLACK_WIN_STRING;
            break;
        case GameState::STALEMATE:
            gameOverText = GOCfg::STALEMATE_STRING;
            break;
        case GameState::DRAW_50_MOVES:
            gameOverText = GOCfg::DRAW_50_MOVES_STRING;
            break;
        case GameState::DRAW_THREEFOLD_REPETITION:
            gameOverText = GOCfg::DRAW_THREEFOLD_REPETITION_STRING;
            break;
        case GameState::DRAW_INSUFFICIENT_MATERIAL:
            gameOverText = GOCfg::DRAW_INSUFFICIENT_MATERIAL_STRING;
            break;
        case GameState::IN_PROGRESS:
            gameOverText = ""; // No game over message
            break;
        default:
            gameOverText = GOCfg::ERROR_STRING;
            break;
    }
    return gameOverText;
}

int ChessAnalysisProgram::getHalfmoveClock() const
{
    return logic.getHalfmoveClock();
}