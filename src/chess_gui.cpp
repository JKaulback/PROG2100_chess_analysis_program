#include "chess_gui.h"


ChessGUI::ChessGUI(ChessLogic& logic) : chessLogic(logic) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess Analysis Program");

    SetTargetFPS(TARGET_FPS); // Set the desired frame rate

    // Load assets
    boardTexture = LoadTexture(BOARD_TEXTURE_PATH); // The chess board is initially 1600x1600 px

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
    for (const std::pair<std::string, Texture2D>& pieceTexture : pieceTextures) {
        UnloadTexture(pieceTexture.second);
    }
    UnloadTexture(boardTexture);
    CloseWindow(); // Close window and OpenGL context
}

void ChessGUI::initPieceTextures() {
    // Loop through the logical board
    for (int rank = MIN_RANK; rank <= MAX_RANK ; rank++) {
        for (int file = MIN_FILE; file <= MAX_FILE; file++) {
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

void ChessGUI::loadPieceTexture(const std::string& pieceString) {
    std::string path = PIECES_TEXTURE_PATH + pieceString + ".png";

    pieceTextures[pieceString] = LoadTexture(path.c_str());
}

void ChessGUI::draw() const {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // Draw the chess board
    DrawTextureEx(boardTexture, {0, 0}, 0.0f, BOARD_SCALE, WHITE);

    // Draw all chess pieces in play
    drawChessPieces();

    EndDrawing();
}

void ChessGUI::update() {
    const Vector2 mousePos = GetMousePosition();
    
    // Check for drag start
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Check if dragging a piece
        const Vector2 boardPos = screenPosToBoardPos(mousePos);
        const ChessPiece selectedPiece = chessLogic.getPieceAt(static_cast<int>(boardPos.y), static_cast<int>(boardPos.x));
        
        if (selectedPiece.isPlayable()) {
            // Set state variables for dragging
            isDragging = true;
            draggedPieceRank = static_cast<int>(boardPos.y);
            draggedPieceFile = static_cast<int>(boardPos.x);
            draggedPiece = selectedPiece;
            
            // Calculate the offset for the piece center to the mouse
            const Vector2 pieceScreenPos = boardPosToScreenPos(boardPos);
            const Vector2 pieceCenterPos = {
                pieceScreenPos.x + PIECE_SIZE / 2.0f,
                pieceScreenPos.y + PIECE_SIZE / 2.0f
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
        const Vector2 boardDropPos = screenPosToBoardPos(mousePos);
        const int dropFile = static_cast<int>(boardDropPos.x);
        const int dropRank = static_cast<int>(boardDropPos.y);

        // Attempt to make a move (move validation is handled in logic)
        chessLogic.makeMove(draggedPieceRank, draggedPieceFile, dropRank, dropFile);

        // Stop dragging regardless of move success
        resetDragState();
    }

}

Vector2 ChessGUI::screenPosToBoardPos(Vector2 screenPos) const {
    int file = static_cast<int>((screenPos.x - BOARD_OFFSET_X) / SQUARE_SIZE);
    int rank = MAX_RANK - static_cast<int>((screenPos.y - BOARD_OFFSET_Y) / SQUARE_SIZE);

    return {static_cast<float>(file), static_cast<float>(rank)};
}

Vector2 ChessGUI::boardPosToScreenPos(Vector2 boardPos) const {
    return Vector2{
        BOARD_OFFSET_X + (boardPos.x * SQUARE_SIZE) + CENTERED_VALUE,
        BOARD_OFFSET_Y + ((MAX_RANK - boardPos.y) * SQUARE_SIZE) + CENTERED_VALUE,

    };
}

void ChessGUI::drawChessPieces() const {
    // Draw all playable pieces not being dragged
    for (int rank = MIN_RANK; rank <= MAX_RANK; rank++) {
        for (int file = MIN_FILE; file <= MAX_FILE; file++) {
            // Skip piece if being dragged
            if (isDragging && draggedPieceRank == rank && draggedPieceFile == file) {
                continue;
            }
            // Draw playable pieces
            ChessPiece currentPiece = chessLogic.getPieceAt(rank, file);
            if (currentPiece.isPlayable()) {
                std::string pieceString = currentPiece.pieceToString();
                const Vector2 screenPos = boardPosToScreenPos({static_cast<float>(file), static_cast<float>(rank)});
                DrawTextureEx(pieceTextures.at(pieceString), screenPos, 0.0f, PIECE_SCALE, WHITE);
            }
        }
    }
    // Draw the dragged piece at mouse position
    if (isDragging) {
        const Vector2 mousePos = GetMousePosition();
        std::string pieceString = draggedPiece.pieceToString();

        // Calculate draw position
        const Vector2 drawPos = {
            mousePos.x - dragOffset.x - PIECE_SIZE / 2.0f,
            mousePos.y - dragOffset.y - PIECE_SIZE / 2.0f
        };

        // Draw the piece being dragged
        DrawTextureEx(pieceTextures.at(pieceString), drawPos, 0.0f, PIECE_SCALE, WHITE);
    }
}

void ChessGUI::resetDragState() {
    isDragging = false;
    draggedPieceRank = -1;
    draggedPieceFile = -1;
    draggedPiece = ChessPiece();
}