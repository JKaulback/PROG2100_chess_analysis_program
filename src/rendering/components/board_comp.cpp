#include "board_comp.h"
#include "../../config/config.h"

namespace BoardCfg = Config::Board;
namespace PieceCfg = Config::Pieces;

BoardComp::BoardComp(const ChessAnalysisProgram& controller) :
    controller(controller), texturesLoaded(false) {
    // Textures will be loaded lazily when first needed
}

BoardComp::~BoardComp() {
    // Only unload textures if they were loaded
    if (texturesLoaded) {
        // Unload all the piece textures
        for (const std::pair<std::string, Texture2D>& pieceTexture : pieceTextures) 
            UnloadTexture(pieceTexture.second);
        UnloadTexture(boardTexture); // Unload board texture
    }
}

void BoardComp::draw() const {
    // Ensure textures are loaded before drawing
    if (!texturesLoaded) {
        const_cast<BoardComp*>(this)->initTextures();
        const_cast<BoardComp*>(this)->texturesLoaded = true;
    }
    
    // Draw the chess board
    drawBoard();
    
    // Draw coordinate labels
    drawCoordinates();

    // Draw all chess pieces in play
    drawPieces();

    // Draw dragging piece
    drawDraggedPiece();

    // Draw captured pieces
    drawCapturedPieces();
}

Vector2 BoardComp::screenPosToBoardPos(const Vector2 pos) const {
    int file = static_cast<int>((pos.x - BoardCfg::OFFSET_X) / BoardCfg::SQUARE_SIZE);
    int rank = BoardCfg::MAX_RANK - static_cast<int>((pos.y - BoardCfg::OFFSET_Y) / BoardCfg::SQUARE_SIZE);

    return {
        static_cast<float>(file), 
        static_cast<float>(rank)
    };
}

Vector2 BoardComp::boardPosToScreenPos(const Vector2 pos) const {
    return {
        BoardCfg::OFFSET_X + (pos.x * BoardCfg::SQUARE_SIZE) + PieceCfg::CENTER_OFFSET,
        BoardCfg::OFFSET_Y + ((BoardCfg::MAX_RANK - pos.y) * BoardCfg::SQUARE_SIZE) + PieceCfg::CENTER_OFFSET,
    };
}

float BoardComp::getSquareSize() const {
    return BoardCfg::SQUARE_SIZE;
}

float BoardComp::getPieceSize() const {
    return PieceCfg::SIZE;
}

void BoardComp::initTextures() {
    // Load board
    boardTexture = LoadTexture(BoardCfg::TEXTURE_PATH); // The chess board is initially 1600x1600 px
    
    // Load piece textures
    for (char piece : BoardCfg::VALID_PIECES) {
        initPieceTexture(piece);
    }
}

void BoardComp::initPieceTexture(const char piece) {
    // Get the string for the piece path
    std::string pieceString = controller.pieceToTextureString(piece);
    std::string path = PieceCfg::TEXTURE_PATH + pieceString + ".png";
    // Load the piece
    pieceTextures[pieceString] = LoadTexture(path.c_str());
}

void BoardComp::drawBoard() const {
    // Draw the chess board with proper offset for margins
    DrawTextureEx(boardTexture, {BoardCfg::OFFSET_X, BoardCfg::OFFSET_Y}, 0.0f, BoardCfg::SCALE, WHITE);
}

void BoardComp::drawCoordinates() const {
    const int fontSize = 20;
    const int labelOffset = 25;
    const Color labelColor = DARKGRAY;
    
    // Draw file letters (a-h) at bottom and top
    for (int file = 0; file < 8; file++) {
        char fileLetter = 'a' + file;
        const float xPos = BoardCfg::OFFSET_X + (file * BoardCfg::SQUARE_SIZE) + (BoardCfg::SQUARE_SIZE / 2) - (fontSize / 4);
        
        // Bottom labels
        const float bottomY = BoardCfg::OFFSET_Y + BoardCfg::SIZE + 5;
        DrawText(TextFormat("%c", fileLetter), static_cast<int>(xPos), static_cast<int>(bottomY), fontSize, labelColor);
        
        // Top labels
        const float topY = BoardCfg::OFFSET_Y - labelOffset;
        DrawText(TextFormat("%c", fileLetter), static_cast<int>(xPos), static_cast<int>(topY), fontSize, labelColor);
    }
    
    // Draw rank numbers (1-8) at left and right
    for (int rank = 0; rank < 8; rank++) {
        int rankNumber = rank + 1;
        const float yPos = BoardCfg::OFFSET_Y + ((7 - rank) * BoardCfg::SQUARE_SIZE) + (BoardCfg::SQUARE_SIZE / 2) - (fontSize / 2);
        
        // Left labels
        const float leftX = BoardCfg::OFFSET_X - labelOffset;
        DrawText(TextFormat("%d", rankNumber), static_cast<int>(leftX), static_cast<int>(yPos), fontSize, labelColor);
        
        // Right labels
        const float rightX = BoardCfg::OFFSET_X + BoardCfg::SIZE + 5;
        DrawText(TextFormat("%d", rankNumber), static_cast<int>(rightX), static_cast<int>(yPos), fontSize, labelColor);
    }
}

void BoardComp::drawPieces() const {
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
}

void BoardComp::drawDraggedPiece() const {
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
}

void BoardComp::drawCapturedPieces() const {
    // Draw captured pieces (white at top, black at bottom)
    std::vector<char> capturedPieces = controller.getCapturedPieces();
    if (!capturedPieces.empty()) {
        int numBlack = 0, numWhite = 0; // To track which column to draw in
        for (int pieceIndex = 0; pieceIndex < capturedPieces.size(); pieceIndex++) {
            // Set up variables to draw the piece
            char piece = capturedPieces[pieceIndex];
            std::string pieceString = controller.pieceToTextureString(piece);
            std::pair<int, int> position;

            char pieceOwner = controller.getPieceOwner(piece);
            if (pieceOwner == 'w') {
                position = getCapturedPiecePosition(numWhite++, pieceOwner);
            } else { // Do the same but for black
                position = getCapturedPiecePosition(numBlack++, pieceOwner);
            }
            // Calculate the x on screen position
            float xPos = PieceCfg::CAPTURED_OFFSET_X + 
                (PieceCfg::CAPTURED_STEP * position.first);
            float yPos = static_cast<float>(position.second);
            // Draw the captured piece
            DrawTextureEx(pieceTextures.at(pieceString), {xPos, yPos}, 0.0f, 
                PieceCfg::CAPTURED_SCALE, WHITE);
        }
    }
}

std::pair<int, int> BoardComp::getCapturedPiecePosition(const int numPieces, const char pieceOwner) const {
    // When drawing more than the config max number, move to next col
    int col = 
        (numPieces <= PieceCfg::MAX_CAPTURED_IN_ROW) ? 
        0 : 
        1;
    // Determine the y grid location
    float numStepsY = 
        (numPieces <= PieceCfg::MAX_CAPTURED_IN_ROW) ?
        numPieces : 
        numPieces - PieceCfg::MAX_CAPTURED_IN_ROW - 1;
    // Calculate the y on screen position
    int yPos = 
        (pieceOwner == 'w') ?
        (PieceCfg::CAPTURED_OFFSET_Y_WHITE + PieceCfg::CAPTURED_STEP * numStepsY) :
        (PieceCfg::CAPTURED_OFFSET_Y_BLACK - PieceCfg::CAPTURED_STEP * numStepsY);

    return {col, yPos};
}