#include "chess_analysis_program.h"

ChessAnalysisProgram::ChessAnalysisProgram() : 
    logic{}, gui{*this}, inputHandler{}, moveValidator{}
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
    // 1. Validate the move using the validator
    MoveResult validationResult = moveValidator.validateMove(logic, srcRank, srcFile, destRank, destFile);
    
    // 2. If valid, execute the move and switch turns
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
        return true;
    }
    
    // 3. Move was invalid - return false
    return false;
}

bool ChessAnalysisProgram::isValidMoveResult(MoveResult result) const {
    return result == MoveResult::VALID || 
           result == MoveResult::VALID_CASTLE_KINGSIDE || 
           result == MoveResult::VALID_CASTLE_QUEENSIDE ||
           result == MoveResult::VALID_EN_PASSANT ||
           result == MoveResult::VALID_PROMOTION;
}
