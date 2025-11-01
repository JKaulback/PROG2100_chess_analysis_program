#include "chess_input_handler.h"
#include "../rendering/chess_gui.h"
#include "../application/chess_analysis_program.h"

ChessInputHandler::ChessInputHandler() 
{
    // Initialize drag state
    resetDragState();
}

ChessInputHandler::~ChessInputHandler() 
{}

void ChessInputHandler::handleInput(ChessAnalysisProgram& controller, const ChessGUI& gui) 
{
    const Vector2 mousePos = GetMousePosition();
    
    // Check for drag start if the game is in progress
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !controller.isGameOver()) 
    {
        // Check if dragging a piece
        const Vector2 boardPos = gui.screenPosToBoardPos(mousePos);
        const ChessLogic::Piece selectedPiece = controller.getPieceAt(static_cast<int>(boardPos.y), static_cast<int>(boardPos.x));

        if (selectedPiece != ChessLogic::Piece::EMPTY) 
        {
            // Set state variables for dragging
            isDragging = true;
            draggedPieceRank = static_cast<int>(boardPos.y);
            draggedPieceFile = static_cast<int>(boardPos.x);
            draggedPiece = selectedPiece;
            
            // Calculate the offset for the piece center to the mouse
            const Vector2 pieceScreenPos = gui.boardPosToScreenPos(boardPos);
            const Vector2 pieceCenterPos = 
            {
                pieceScreenPos.x + gui.getPieceSize() / 2.0f,
                pieceScreenPos.y + gui.getPieceSize() / 2.0f
            };
            dragOffset = 
            {
                mousePos.x - pieceCenterPos.x,
                mousePos.y - pieceCenterPos.y
            };
        }
    }
    // Check for piece drop
    if (isDragging && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) 
    {
        // Get the position of the dropped piece
        const Vector2 boardDropPos = gui.screenPosToBoardPos(mousePos);
        const int dropFile = static_cast<int>(boardDropPos.x);
        const int dropRank = static_cast<int>(boardDropPos.y);

        // Attempt move through controller (includes validation)
        bool moveSuccessful = controller.attemptMove(draggedPieceRank, draggedPieceFile, dropRank, dropFile);
        
        // Optional: Handle failed moves with feedback
        if (!moveSuccessful) {
            // Future: Could add visual/audio feedback for invalid moves
            // std::string errorMsg = controller.getMoveValidationMessage(draggedPieceRank, draggedPieceFile, dropRank, dropFile);
            // std::cout << "Invalid move: " << errorMsg << std::endl;
        }

        // Stop dragging regardless of move success
        resetDragState();
    }
}

void ChessInputHandler::resetDragState() 
{
    isDragging = false;
    draggedPieceRank = -1;
    draggedPieceFile = -1;
    dragOffset = {0, 0};
    draggedPiece = ChessLogic::Piece::EMPTY;
}

bool ChessInputHandler::getIsDragging() const 
{
    return isDragging;
}

int ChessInputHandler::getDraggedPieceRank() const 
{
    return draggedPieceRank;
}

int ChessInputHandler::getDraggedPieceFile() const 
{
    return draggedPieceFile;
}

Vector2 ChessInputHandler::getDragOffset() const 
{
    return dragOffset;
}

ChessLogic::Piece ChessInputHandler::getDraggedPiece() const 
{
    return draggedPiece;
}

