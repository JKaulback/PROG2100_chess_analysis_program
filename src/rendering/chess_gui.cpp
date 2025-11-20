#include "chess_gui.h"
#include "../application/chess_analysis_program.h"

// Namespace aliases for cleaner code while maintaining clarity
namespace WinCfg = Config::Window;
namespace BoardCfg = Config::Board;
namespace PieceCfg = Config::Pieces;
namespace GOCfg = Config::GameOver;
namespace StatsCfg = Config::GameStatistics;
namespace CtrlCfg = Config::Controls;

ChessGUI::ChessGUI(const ChessAnalysisProgram& ctrl) : controller(ctrl) {
    InitWindow(WinCfg::WIDTH, WinCfg::HEIGHT, WinCfg::TITLE);

    SetTargetFPS(WinCfg::TARGET_FPS); // Set the desired frame rate

    // Load assets
    boardTexture = LoadTexture(BoardCfg::TEXTURE_PATH); // The chess board is initially 1600x1600 px

    // Initialize pieces from the logic board
    initPieceTextures();

    // Toggle full screen
    ToggleFullscreen();

    // Assume UCI engine not started on startup 
    // (sync not available until engine is configured)
    isUCIEngineRunning =  false;
}

ChessGUI::~ChessGUI() {
    // Unload all the piece textures
    for (const std::pair<std::string, Texture2D>& pieceTexture : pieceTextures) 
        UnloadTexture(pieceTexture.second);
    UnloadTexture(boardTexture); // Unload board texture
    CloseWindow(); // Close window and OpenGL context
}

void ChessGUI::initPieceTextures() {
    // Loop through the logical board
    for (int rank = BoardCfg::MIN_RANK; rank <= BoardCfg::MAX_RANK; rank++) {
        for (int file = BoardCfg::MIN_FILE; file <= BoardCfg::MAX_FILE; file++) {
            // Check the playable pieces
            char currentPiece = controller.getPieceAt(rank, file);
            if (currentPiece != BoardCfg::EMPTY) {
                // Ensure the texture hasn't been loaded yet
                std::string pieceString = controller.pieceToTextureString(currentPiece);
                if (!pieceTextures.count(pieceString))
                    loadPieceTexture(pieceString); // Load the texture
            }
        }
    }
}

void ChessGUI::loadPieceTexture(const std::string& pieceString) {
    std::string path = PieceCfg::TEXTURE_PATH + pieceString + ".png";

    pieceTextures[pieceString] = LoadTexture(path.c_str());
}

void ChessGUI::draw() const {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // Draw the chess board
    DrawTextureEx(boardTexture, {0, 0}, 0.0f, BoardCfg::SCALE, WHITE);

    // Draw all chess pieces in play
    drawChessPieces();

    // Draw the stats
    drawStats();

    // Draw the controls
    drawControls();

    // Check for game over
    if (controller.isGameOver())
        drawGameOverScreen();

    EndDrawing();
}

Vector2 ChessGUI::screenPosToBoardPos(const Vector2 screenPos) const {
    int file = static_cast<int>((screenPos.x - BoardCfg::OFFSET_X) / BoardCfg::SQUARE_SIZE);
    int rank = BoardCfg::MAX_RANK - static_cast<int>((screenPos.y - BoardCfg::OFFSET_Y) / BoardCfg::SQUARE_SIZE);

    return {
        static_cast<float>(file), 
        static_cast<float>(rank)
    };
}

Vector2 ChessGUI::boardPosToScreenPos(const Vector2 boardPos) const {
    return {
        BoardCfg::OFFSET_X + (boardPos.x * BoardCfg::SQUARE_SIZE) + PieceCfg::CENTER_OFFSET,
        BoardCfg::OFFSET_Y + ((BoardCfg::MAX_RANK - boardPos.y) * BoardCfg::SQUARE_SIZE) + PieceCfg::CENTER_OFFSET,
    };
}

void ChessGUI::drawChessPieces() const {
    // Draw all playable pieces not being dragged
    for (int rank = BoardCfg::MIN_RANK; rank <= BoardCfg::MAX_RANK; rank++) {
        for (int file = BoardCfg::MIN_FILE; file <= BoardCfg::MAX_FILE; file++) {
            // Skip piece if being dragged
            if (controller.getIsDragging() && 
                    controller.getDraggedPieceRank() == rank && 
                    controller.getDraggedPieceFile() == file)
                continue;
            // Draw playable pieces
            char currentPiece = controller.getPieceAt(rank, file);
            if (currentPiece != BoardCfg::EMPTY) {
                std::string pieceString = controller.pieceToTextureString(currentPiece);
                const Vector2 screenPos = 
                    boardPosToScreenPos({
                        static_cast<float>(file), 
                        static_cast<float>(rank)
                    });
                DrawTextureEx(pieceTextures.at(pieceString), screenPos, 0.0f, 
                    PieceCfg::SCALE, WHITE);
            }
        }
    }
    // Draw the dragged piece at mouse position
    if (controller.getIsDragging()) {
        const Vector2 mousePos = GetMousePosition();
        std::string pieceString = controller.pieceToTextureString(controller.getDraggedPiece());
        Vector2 dragOffset = controller.getDragOffset();

        // Calculate draw position
        const Vector2 drawPos = {
            mousePos.x - dragOffset.x - PieceCfg::SIZE / 2.0f,
            mousePos.y - dragOffset.y - PieceCfg::SIZE / 2.0f
        };

        // Draw the piece being dragged
        DrawTextureEx(pieceTextures.at(pieceString), drawPos, 0.0f,
            PieceCfg::SCALE, WHITE);
    }
    // Draw captured pieces (white at top, black at bottom)
    std::vector<char> capturedPieces = controller.getCapturedPieces();
    if (!capturedPieces.empty()) {
        int numBlack = 0, numWhite = 0; // To track which column to draw in
        for (int pieceIndex = 0; pieceIndex < capturedPieces.size(); pieceIndex++) {
            // Set up variables to draw the piece
            char piece = capturedPieces[pieceIndex];
            std::string pieceString = controller.pieceToTextureString(piece);
            float yPos, col;

            if (controller.getPieceOwner(piece) == 'w') {
                // When drawing more than the config max number, move to next col
                col = 
                    (numWhite <= PieceCfg::MAX_CAPTURED_IN_ROW) ? 
                    0 : 
                    1;
                // Determine the y grid location
                float numStepsY = 
                    (numWhite <= PieceCfg::MAX_CAPTURED_IN_ROW) ?
                    numWhite : 
                    numWhite - PieceCfg::MAX_CAPTURED_IN_ROW;
                // Calculate the y on screen position
                yPos = PieceCfg::CAPTURED_OFFSET_Y_WHITE + 
                    (PieceCfg::CAPTURED_STEP * numStepsY);
                // Iterate number of white pieces drawn
                numWhite++;
            } else { // Do the same but for black
                col = 
                    (numBlack <= PieceCfg::MAX_CAPTURED_IN_ROW) ? 
                    0 : 
                    1;
                float numStepsY = 
                    (numBlack <= PieceCfg::MAX_CAPTURED_IN_ROW) ?
                    numBlack : 
                    numBlack - PieceCfg::MAX_CAPTURED_IN_ROW;
                yPos = PieceCfg::CAPTURED_OFFSET_Y_BLACK - 
                    (PieceCfg::CAPTURED_STEP * numStepsY);
                numBlack++;
            }
            // Calculate the x on screen position
            float xPos = PieceCfg::CAPTURED_OFFSET_X + 
                (PieceCfg::CAPTURED_STEP * col);
            // Draw the captured piece
            DrawTextureEx(pieceTextures.at(pieceString), {xPos, yPos}, 0.0f, 
                PieceCfg::CAPTURED_SCALE, WHITE);
        }
    }
}

void ChessGUI::drawGameOverScreen() const {
    // Get the text to display
    std::string gameOverText = controller.getGameOverString();

    // Calculate text size
    Vector2 textSize = MeasureTextEx(GetFontDefault(), gameOverText.c_str(), 
        GOCfg::STATE_FONT_SIZE_PX, GOCfg::STATE_FONT_SPACING);

    // Center text position in window
    Vector2 textPos = {
        (GetScreenWidth() / 2.0f) - (textSize.x / 2.0f),
        (GetScreenHeight() / 2.0f) - (textSize.y / 2.0f)
    };

    // Draw game over background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GOCfg::BACKGROUND_COLOR);

    // Draw text
    DrawText(gameOverText.c_str(), (int)textPos.x, (int)textPos.y, 
        GOCfg::STATE_FONT_SIZE_PX, GOCfg::STATE_FONT_COLOR);
}

float ChessGUI::getSquareSize() const {
    return BoardCfg::SQUARE_SIZE;
}

float ChessGUI::getPieceSize() const {
    return PieceCfg::SIZE;
}

void ChessGUI::setIsUCIEngineRunning(const bool isRunning) {
    isUCIEngineRunning = isRunning;
}

void ChessGUI::drawStats() const{
    drawHalfMoveClock(0);
    // drawBoardState(12); // FOR DEBUGGING
}

void ChessGUI::drawControls() const {
    drawEngineControls();
}

void ChessGUI::drawHalfMoveClock(const int statIndex) const {
    DrawText(TextFormat("HALFMOVE CLOCK: %i", controller.getHalfmoveClock()),
        StatsCfg::START_DRAW_X, 
        StatsCfg::START_DRAW_Y + (StatsCfg::DRAW_STEP_Y * statIndex),
        StatsCfg::STATS_FONT_SIZE_PX, StatsCfg::STATS_FONT_COLOR);
}

void ChessGUI::drawBoardState(const int statIndex) const {
    DrawText(TextFormat("BOARD STATE:\n%s", controller.getCurrentFENString().c_str()),
        StatsCfg::START_DRAW_X - 300,
        StatsCfg::START_DRAW_Y + (StatsCfg::DRAW_STEP_Y * statIndex),
        StatsCfg::STATS_FONT_SIZE_PX, StatsCfg::STATS_FONT_COLOR);
}

void ChessGUI::drawEngineControls() const {
    std::string engineControlsText =
        (isUCIEngineRunning) ?
        "Press (x) to stop move analysis" :
        "Press (x) to start move analysis";
    
    DrawText(engineControlsText.c_str(),
        CtrlCfg::START_DRAW_X - 300,
        CtrlCfg::START_DRAW_Y,
        CtrlCfg::FONT_SIZE_PX, CtrlCfg::FONT_COLOR);
}