#include "piece_renderer.h"
#include "texture_manager.h"
#include "../../application/chess_analysis_program.h"
#include "../../config/config.h"

namespace BoardCfg = Config::Board;
namespace PieceCfg = Config::Pieces;

PieceRenderer::PieceRenderer(const ChessAnalysisProgram& controller, const TextureManager& textureManager) :
    controller(controller), textureManager(textureManager) {
}

void PieceRenderer::draw() const {
    drawPieces();
    drawDraggedPiece();
    drawCapturedPieces();
}

Vector2 PieceRenderer::screenPosToBoardPos(const Vector2 pos) const {
    int file = static_cast<int>((pos.x - BoardCfg::OFFSET_X) / BoardCfg::SQUARE_SIZE);
    int rank = BoardCfg::MAX_RANK - static_cast<int>((pos.y - BoardCfg::OFFSET_Y) / BoardCfg::SQUARE_SIZE);

    return {
        static_cast<float>(file), 
        static_cast<float>(rank)
    };
}

Vector2 PieceRenderer::boardPosToScreenPos(const Vector2 pos) const {
    return {
        BoardCfg::OFFSET_X + (pos.x * BoardCfg::SQUARE_SIZE) + PieceCfg::CENTER_OFFSET,
        BoardCfg::OFFSET_Y + ((BoardCfg::MAX_RANK - pos.y) * BoardCfg::SQUARE_SIZE) + PieceCfg::CENTER_OFFSET,
    };
}

void PieceRenderer::drawPieces() const {
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
                DrawTextureEx(textureManager.getPieceTexture(pieceString), screenPos, 0.0f, 
                    PieceCfg::SCALE, WHITE);
            }
        }
    }
}

void PieceRenderer::drawDraggedPiece() const {
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
        DrawTextureEx(textureManager.getPieceTexture(pieceString), drawPos, 0.0f,
            PieceCfg::SCALE, WHITE);
    }
}

void PieceRenderer::drawCapturedPieces() const {
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
            DrawTextureEx(textureManager.getPieceTexture(pieceString), {xPos, yPos}, 0.0f, 
                PieceCfg::CAPTURED_SCALE, WHITE);
        }
    }
}

std::pair<int, int> PieceRenderer::getCapturedPiecePosition(const int numPieces, const char pieceOwner) const {
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