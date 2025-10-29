#include "chess_input_handler.h"
#include "../rendering/chess_gui.h"

ChessInputHandler::ChessInputHandler() {
    // Initialize drag state
    resetDragState();
}

ChessInputHandler::~ChessInputHandler() {}

void ChessInputHandler::handleInput(ChessLogic& logic, const ChessGUI& gui) {
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

void ChessInputHandler::resetDragState() {
    isDragging = false;
    draggedPieceRank = -1;
    draggedPieceFile = -1;
    dragOffset = {0, 0};
    draggedPiece = ChessLogic::Piece::EMPTY;
}