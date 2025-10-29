#include "chess_analysis_program.h"

ChessAnalysisProgram::ChessAnalysisProgram() : 
    logic{}, gui{*this}, inputHandler{} 
{}

ChessAnalysisProgram::~ChessAnalysisProgram() 
{}

void ChessAnalysisProgram::run() 
{

    // Main loop
    while (!WindowShouldClose()) 
    { // Detect window close button or ESC key
        
        inputHandler.handleInput(logic, gui); // Input handler processes input
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