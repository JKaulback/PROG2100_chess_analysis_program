#include "chess_gui.h"
#include "../application/chess_analysis_program.h"

// Namespace aliases for cleaner code while maintaining clarity
namespace WinCfg = Config::Window;
namespace BoardCfg = Config::Board;
namespace PieceCfg = Config::Pieces;


ChessGUI::ChessGUI(const ChessAnalysisProgram& ctrl) : controller(ctrl) 
{
    InitWindow(WinCfg::WIDTH, WinCfg::HEIGHT, WinCfg::TITLE);

    SetTargetFPS(WinCfg::TARGET_FPS); // Set the desired frame rate

    // Load assets
    boardTexture = LoadTexture(BoardCfg::TEXTURE_PATH); // The chess board is initially 1600x1600 px

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
    for (int rank = BoardCfg::MIN_RANK; rank <= BoardCfg::MAX_RANK ; rank++) 
    {
        for (int file = BoardCfg::MIN_FILE; file <= BoardCfg::MAX_FILE; file++) 
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
    std::string path = PieceCfg::TEXTURE_PATH + pieceString + ".png";

    pieceTextures[pieceString] = LoadTexture(path.c_str());
}

void ChessGUI::draw() const 
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // Draw the chess board
    DrawTextureEx(boardTexture, {0, 0}, 0.0f, Config::Board::SCALE, WHITE);

    // Draw all chess pieces in play
    drawChessPieces();

    EndDrawing();
}



Vector2 ChessGUI::screenPosToBoardPos(Vector2 screenPos) const 
{
    int file = static_cast<int>((screenPos.x - Config::Board::OFFSET_X) / Config::Board::SQUARE_SIZE);
    int rank = Config::Board::MAX_RANK - static_cast<int>((screenPos.y - Config::Board::OFFSET_Y) / Config::Board::SQUARE_SIZE);

    return 
    {
        static_cast<float>(file), static_cast<float>(rank)
    };
}

Vector2 ChessGUI::boardPosToScreenPos(Vector2 boardPos) const 
{
    return Vector2
    {
        Config::Board::OFFSET_X + (boardPos.x * Config::Board::SQUARE_SIZE) + Config::Pieces::CENTER_OFFSET,
        Config::Board::OFFSET_Y + ((Config::Board::MAX_RANK - boardPos.y) * Config::Board::SQUARE_SIZE) + Config::Pieces::CENTER_OFFSET,

    };
}

void ChessGUI::drawChessPieces() const 
{
    // Draw all playable pieces not being dragged
    for (int rank = Config::Board::MIN_RANK; rank <= Config::Board::MAX_RANK; rank++) 
    {
        for (int file = Config::Board::MIN_FILE; file <= Config::Board::MAX_FILE; file++) 
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
                DrawTextureEx(pieceTextures.at(pieceString), screenPos, 0.0f, Config::Pieces::SCALE, WHITE);
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
            mousePos.x - dragOffset.x - Config::Pieces::SIZE / 2.0f,
            mousePos.y - dragOffset.y - Config::Pieces::SIZE / 2.0f
        };

        // Draw the piece being dragged
        DrawTextureEx(pieceTextures.at(pieceString), drawPos, 0.0f, Config::Pieces::SCALE, WHITE);
    }
}

float ChessGUI::getSquareSize() const 
{
    return Config::Board::SQUARE_SIZE;
}

float ChessGUI::getPieceSize() const 
{
    return Config::Pieces::SIZE;
}