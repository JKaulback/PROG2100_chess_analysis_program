#include "chess_gui.h"


ChessGUI::ChessGUI(ChessLogic& logic) : chessLogic(logic) {
    // Initialization
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "Chess Analysis Program");

    SetTargetFPS(60); // Set the desired frame rate

    // Load assets
    boardTexture = LoadTexture("src/assets/board.png"); // The chess board is initially 1600x1600 px

    // Initialize pieces from the logic board
    initPieceTextures();

    // Initialize drag state
    isDragging = false;
    draggedPieceRank = -1; // Invalid rank
    draggedPieceFile = -1; // Invalid file
    dragOffset = {0, 0};
    draggedPiece = ChessPiece(); // Empty piece
}

ChessGUI::~ChessGUI() {
    // Unload all the piece textures
    for (const std::pair<std::string, Texture2D> pieceTexture : pieceTextures) {
        UnloadTexture(pieceTexture.second);
    }
    UnloadTexture(boardTexture);
    CloseWindow(); // Close window and OpenGL context
}

void ChessGUI::initPieceTextures() {
    // Loop through the logical board
    for (int rank = minRank; rank <= maxRank ; rank++) {
        for (int file = minFile; file <= maxFile; file++) {
            // Check the playable pieces
            ChessPiece currentPiece = chessLogic.getPieceAt(rank, file);
            if (currentPiece.isPlayable()) {
                // Ensure the texture hasn't been loaded yet
                std::string pieceString = currentPiece.pieceToString();
                if (!pieceTextures.count(pieceString)) {
                    // Load the texture
                    loadPieceTexture(pieceString);
                }
            }
        }
    }
}

void ChessGUI::loadPieceTexture(const std::string pieceString) {
    std::string path = "src/assets/chess_pieces/";
    path.append(pieceString);
    path.append(".png");

    Texture2D pieceTexture = LoadTexture(path.c_str());
    pieceTextures[pieceString] = pieceTexture;
}

void ChessGUI::draw() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // Draw the chess board
    DrawTextureEx(this->boardTexture, {0, 0}, 0.0f, this->boardScale, WHITE);

    // Draw all chess pieces in play
    drawChessPieces();

    EndDrawing();
}

void ChessGUI::update() {
    Vector2 mousePos = GetMousePosition();
    // Check for drag start
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Check if dragging a piece
        Vector2 draggedPiecePos = screenPosToBoardPos(mousePos);
        ChessPiece selectedPiece = chessLogic.getPieceAt(draggedPiecePos.y, draggedPiecePos.x);
        if (selectedPiece.isPlayable()) {
            // Set state variables for dragging
            isDragging = true;
            draggedPieceRank = draggedPiecePos.y;
            draggedPieceFile = draggedPiecePos.x;
            draggedPiece = selectedPiece;
            // Calculate the offset for the piece center to the mouse
            Vector2 pieceScreenPos = boardPosToScreenPos(draggedPiecePos);
            Vector2 pieceCenterPos = {
                pieceScreenPos.x + pieceSize / 2.0f,
                pieceScreenPos.y + pieceSize / 2.0f
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
        Vector2 boardDropPos = screenPosToBoardPos(mousePos);
        int dropFile = static_cast<int>(boardDropPos.x);
        int dropRank = static_cast<int>(boardDropPos.y);

        // Attempt to make a move (move validation is handled in logic)
        chessLogic.makeMove(draggedPieceRank, draggedPieceFile, dropRank, dropFile);

        // Stop dragging regardless of move success
        isDragging = false;
        draggedPieceRank = -1;
        draggedPieceFile = -1;
        draggedPiece = ChessPiece();
    }

}

Vector2 ChessGUI::screenPosToBoardPos(Vector2 screenPos) const {
    int file = static_cast<int>((screenPos.x - boardOffsetX) / squareSize);
    int rank = maxRank - static_cast<int>((screenPos.y - boardOffsetY) / squareSize);

    return {static_cast<float>(file), static_cast<float>(rank)};
}

Vector2 ChessGUI::boardPosToScreenPos(Vector2 boardPos) const {
    return Vector2{
        boardOffsetX + (boardPos.x * squareSize) + centeredValue,
        boardOffsetY + ((maxRank - boardPos.y) * squareSize) + centeredValue,

    };
}

void ChessGUI::drawChessPieces() {
    // Draw all playable pieces not being dragged
    for (int rank = minRank; rank <= maxRank; rank++) {
        for (int file = minFile; file <= maxFile; file++) {
            // Skip piece if being dragged
            if (isDragging && draggedPieceRank == rank && draggedPieceFile == file) {
                continue;
            }
            // Draw playable pieces
            ChessPiece currentPiece = chessLogic.getPieceAt(rank, file);
            std::string pieceString = currentPiece.pieceToString();
            if (currentPiece.isPlayable()) {
                Vector2 screenPos = boardPosToScreenPos({static_cast<float>(file), static_cast<float>(rank)});
                DrawTextureEx(pieceTextures[pieceString], screenPos, 0.0f, pieceScale, WHITE);
            }
        }
    }
    // Draw the dragged piece at mouse position
    if (isDragging) {
        Vector2 mousePos = GetMousePosition();
        std::string pieceString = draggedPiece.pieceToString();

        // Calculate draw position
        Vector2 drawPos = {
            mousePos.x - dragOffset.x - pieceSize / 2.0f,
            mousePos.y - dragOffset.y - pieceSize / 2.0f
        };

        // Draw the piece being dragged
        DrawTextureEx(pieceTextures[pieceString], drawPos, 0.0f, pieceScale, WHITE);
    }
}

