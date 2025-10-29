#include "chess_gui.h"
#include "../application/chess_analysis_program.h"


ChessGUI::ChessGUI(const ChessAnalysisProgram& ctrl) : controller(ctrl) 
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess Analysis Program");

    SetTargetFPS(TARGET_FPS); // Set the desired frame rate

    // Load assets
    boardTexture = LoadTexture(BOARD_TEXTURE_PATH); // The chess board is initially 1600x1600 px

    // Initialize pieces from the logic board
    initPieceTextures();

    // Toggle full screen
    ToggleFullscreen();
}

ChessGUI::~ChessGUI() {
    // Unload all the piece textures
    for (const std::pair<std::string, Texture2D>& pieceTexture : pieceTextures) 
    {
        UnloadTexture(pieceTexture.second);
    }
    UnloadTexture(boardTexture);
    CloseWindow(); // Close window and OpenGL context
}

void ChessGUI::initPieceTextures() 
{
    // Loop through the logical board
    for (int rank = MIN_RANK; rank <= MAX_RANK ; rank++) 
    {
        for (int file = MIN_FILE; file <= MAX_FILE; file++) 
        {
            // Check the playable pieces
            ChessLogic::Piece currentPiece = controller.getPieceAt(rank, file);
            if (currentPiece != ChessLogic::Piece::EMPTY) 
            {
                // Ensure the texture hasn't been loaded yet
                std::string pieceString = controller.pieceToString(currentPiece);
                if (!pieceTextures.count(pieceString)) {
                    // Load the texture
                    loadPieceTexture(pieceString);
                }
            }
        }
    }
}

void ChessGUI::loadPieceTexture(const std::string& pieceString) 
{
    std::string path = PIECES_TEXTURE_PATH + pieceString + ".png";

    pieceTextures[pieceString] = LoadTexture(path.c_str());
}

void ChessGUI::draw() const 
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // Draw the chess board
    DrawTextureEx(boardTexture, {0, 0}, 0.0f, BOARD_SCALE, WHITE);

    // Draw all chess pieces in play
    drawChessPieces();

    EndDrawing();
}



Vector2 ChessGUI::screenPosToBoardPos(Vector2 screenPos) const 
{
    int file = static_cast<int>((screenPos.x - BOARD_OFFSET_X) / SQUARE_SIZE);
    int rank = MAX_RANK - static_cast<int>((screenPos.y - BOARD_OFFSET_Y) / SQUARE_SIZE);

    return 
    {
        static_cast<float>(file), static_cast<float>(rank)
    };
}

Vector2 ChessGUI::boardPosToScreenPos(Vector2 boardPos) const 
{
    return Vector2
    {
        BOARD_OFFSET_X + (boardPos.x * SQUARE_SIZE) + CENTERED_VALUE,
        BOARD_OFFSET_Y + ((MAX_RANK - boardPos.y) * SQUARE_SIZE) + CENTERED_VALUE,

    };
}

void ChessGUI::drawChessPieces() const 
{
    // Draw all playable pieces not being dragged
    for (int rank = MIN_RANK; rank <= MAX_RANK; rank++) 
    {
        for (int file = MIN_FILE; file <= MAX_FILE; file++) 
        {
            // Skip piece if being dragged
            if (controller.getIsDragging() && 
                controller.getDraggedPieceRank() == rank && 
                controller.getDraggedPieceFile() == file)
            {
                continue;
            }
            // Draw playable pieces
            ChessLogic::Piece currentPiece = controller.getPieceAt(rank, file);
            if (currentPiece != ChessLogic::Piece::EMPTY) 
            {
                std::string pieceString = controller.pieceToString(currentPiece);
                const Vector2 screenPos = boardPosToScreenPos({static_cast<float>(file), static_cast<float>(rank)});
                DrawTextureEx(pieceTextures.at(pieceString), screenPos, 0.0f, PIECE_SCALE, WHITE);
            }
        }
    }
    // Draw the dragged piece at mouse position
    if (controller.getIsDragging()) 
    {
        const Vector2 mousePos = GetMousePosition();
        std::string pieceString = controller.pieceToString(controller.getDraggedPiece());
        Vector2 dragOffset = controller.getDragOffset();

        // Calculate draw position
        const Vector2 drawPos = 
        {
            mousePos.x - dragOffset.x - PIECE_SIZE / 2.0f,
            mousePos.y - dragOffset.y - PIECE_SIZE / 2.0f
        };

        // Draw the piece being dragged
        DrawTextureEx(pieceTextures.at(pieceString), drawPos, 0.0f, PIECE_SCALE, WHITE);
    }
}

float ChessGUI::getSquareSize() const 
{
    return SQUARE_SIZE;
}

float ChessGUI::getPieceSize() const 
{
    return PIECE_SIZE;
}