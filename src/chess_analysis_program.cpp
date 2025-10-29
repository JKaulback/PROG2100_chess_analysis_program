#include "chess_analysis_program.h"

ChessAnalysisProgram::ChessAnalysisProgram() : logic{}, gui{*this} {
    // Constructor now initializes logic first, then gui with controller reference
    // Initialize drag state
    resetDragState();
}

ChessAnalysisProgram::~ChessAnalysisProgram() {}

void ChessAnalysisProgram::run() {

    // Main loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        
        handleInput(); // Controller handles input
        gui.draw();    // GUI only renders
    }

    // De-initialization
}

// Controller methods - handle user interactions
void ChessAnalysisProgram::handleInput() {
    const Vector2 mousePos = GetMousePosition();
    
    // Check for drag start
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Check if dragging a piece
        const Vector2 boardPos = gui.getScreenPosToBoardPos(mousePos);
        const ChessLogic::Piece selectedPiece = logic.getPieceAt(static_cast<int>(boardPos.y), static_cast<int>(boardPos.x));

        if (selectedPiece != ChessLogic::Piece::EMPTY) {
            // Set state variables for dragging
            isDragging = true;
            draggedPieceRank = static_cast<int>(boardPos.y);
            draggedPieceFile = static_cast<int>(boardPos.x);
            draggedPiece = selectedPiece;
            
            // Calculate the offset for the piece center to the mouse
            const Vector2 pieceScreenPos = gui.getBoardPosToScreenPos(boardPos);
            const Vector2 pieceCenterPos = {
                pieceScreenPos.x + gui.getPieceSize() / 2.0f,
                pieceScreenPos.y + gui.getPieceSize() / 2.0f
            };
            dragOffset = {
                mousePos.x - pieceCenterPos.x,
                mousePos.y - pieceCenterPos.y
            };
        }
    }
    // Check for piece drop
    if (isDragging && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        // Get the position of the dropped piece
        const Vector2 boardDropPos = gui.getScreenPosToBoardPos(mousePos);
        const int dropFile = static_cast<int>(boardDropPos.x);
        const int dropRank = static_cast<int>(boardDropPos.y);

        // Attempt to make a move (move validation is handled in logic)
        logic.makeMove(draggedPieceRank, draggedPieceFile, dropRank, dropFile);

        // Stop dragging regardless of move success
        resetDragState();
    }
}

// Delegate methods to logic
ChessLogic::Piece ChessAnalysisProgram::getPieceAt(int rank, int file) const {
    return logic.getPieceAt(rank, file);
}

void ChessAnalysisProgram::makeMove(int fromRank, int fromFile, int toRank, int toFile) {
    logic.makeMove(fromRank, fromFile, toRank, toFile);
}

std::string ChessAnalysisProgram::pieceToString(ChessLogic::Piece piece) const {
    return logic.pieceToString(piece);
}

// Helper methods
void ChessAnalysisProgram::resetDragState() {
    isDragging = false;
    draggedPieceRank = -1;
    draggedPieceFile = -1;
    dragOffset = {0, 0};
    draggedPiece = ChessLogic::Piece::EMPTY;
}