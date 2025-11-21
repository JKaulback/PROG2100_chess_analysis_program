#include "board_comp.h"
#include "../../application/chess_analysis_program.h"
#include "../../config/config.h"

namespace BoardCfg = Config::Board;
namespace PieceCfg = Config::Pieces;

BoardComp::BoardComp(const ChessAnalysisProgram& controller) :
    controller(controller) {
    // Initialize all rendering components
    textureManager = std::make_unique<TextureManager>(controller);
    boardRenderer = std::make_unique<BoardRenderer>(*textureManager);
    coordinateRenderer = std::make_unique<CoordinateRenderer>(controller);
    pieceRenderer = std::make_unique<PieceRenderer>(controller, *textureManager);
    capturedPiecesRenderer = std::make_unique<CapturedPiecesRenderer>(controller, *textureManager);
}

void BoardComp::draw() const {
    // Ensure textures are loaded before drawing
    if (!textureManager->areTexturesLoaded()) {
        textureManager->loadTextures();
    }
    
    // Draw components in order
    boardRenderer->draw();
    coordinateRenderer->draw();
    pieceRenderer->draw();
    capturedPiecesRenderer->draw();
}

Vector2 BoardComp::screenPosToBoardPos(const Vector2 pos) const {
    return pieceRenderer->screenPosToBoardPos(pos);
}

Vector2 BoardComp::boardPosToScreenPos(const Vector2 pos) const {
    return pieceRenderer->boardPosToScreenPos(pos);
}

float BoardComp::getSquareSize() const {
    return BoardCfg::SQUARE_SIZE;
}

float BoardComp::getPieceSize() const {
    return PieceCfg::SIZE;
}