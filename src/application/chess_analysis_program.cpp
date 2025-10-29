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
    auto validationResult = moveValidator.validateMove(logic, srcRank, srcFile, destRank, destFile);
    
    // 2. If valid, execute the move through the logic
    if (validationResult == MoveResult::VALID) {
        logic.executeMove(srcRank, srcFile, destRank, destFile);
        return true;
    }
    
    // 3. Move was invalid - return false
    return false;
}

MoveResult ChessAnalysisProgram::validateMove(int srcRank, int srcFile, int destRank, int destFile) const 
{
    return moveValidator.validateMove(logic, srcRank, srcFile, destRank, destFile);
}

std::string ChessAnalysisProgram::getMoveValidationMessage(int srcRank, int srcFile, int destRank, int destFile) const 
{
    auto result = moveValidator.validateMove(logic, srcRank, srcFile, destRank, destFile);
    return moveValidator.resultToString(result);
}