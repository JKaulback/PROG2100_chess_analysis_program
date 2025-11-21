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
}

Vector2 PieceRenderer::screenPosToBoardPos(const Vector2 pos) const {
    int file = static_cast<int>((pos.x - BoardCfg::OFFSET_X) / BoardCfg::SQUARE_SIZE);
    int rank = BoardCfg::MAX_RANK - static_cast<int>((pos.y - BoardCfg::OFFSET_Y) / BoardCfg::SQUARE_SIZE);
    
    // Flip coordinates if board is flipped (viewing from black's perspective)
    if (controller.getBoardFlipped()) {
        file = BoardCfg::MAX_FILE - file;
        rank = BoardCfg::MAX_RANK - rank;
    }

    return {
        static_cast<float>(file), 
        static_cast<float>(rank)
    };
}

Vector2 PieceRenderer::boardPosToScreenPos(const Vector2 pos) const {
    float displayFile = pos.x;
    float displayRank = pos.y;
    
    // Flip coordinates if board is flipped (viewing from black's perspective)
    if (controller.getBoardFlipped()) {
        displayFile = BoardCfg::MAX_FILE - pos.x;
        displayRank = BoardCfg::MAX_RANK - pos.y;
    }
    
    return {
        BoardCfg::OFFSET_X + (displayFile * BoardCfg::SQUARE_SIZE) + PieceCfg::CENTER_OFFSET,
        BoardCfg::OFFSET_Y + ((BoardCfg::MAX_RANK - displayRank) * BoardCfg::SQUARE_SIZE) + PieceCfg::CENTER_OFFSET,
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

