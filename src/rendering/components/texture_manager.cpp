#include "texture_manager.h"
#include "../../application/chess_analysis_program.h"
#include "../../config/config.h"

namespace BoardCfg = Config::Board;
namespace PieceCfg = Config::Pieces;

TextureManager::TextureManager(const ChessAnalysisProgram& controller) :
    controller(controller), texturesLoaded(false) {
}

TextureManager::~TextureManager() {
    if (texturesLoaded) {
        // Unload all the piece textures
        for (const std::pair<std::string, Texture2D>& pieceTexture : pieceTextures) 
            UnloadTexture(pieceTexture.second);
        UnloadTexture(boardTexture);
    }
}

void TextureManager::loadTextures() {
    if (texturesLoaded) return;
    
    // Load board
    boardTexture = LoadTexture(BoardCfg::TEXTURE_PATH);
    
    // Load piece textures
    for (char piece : BoardCfg::VALID_PIECES) {
        loadPieceTexture(piece);
    }
    
    texturesLoaded = true;
}

bool TextureManager::areTexturesLoaded() const {
    return texturesLoaded;
}

const Texture2D& TextureManager::getBoardTexture() const {
    return boardTexture;
}

const Texture2D& TextureManager::getPieceTexture(const std::string& pieceString) const {
    return pieceTextures.at(pieceString);
}

void TextureManager::loadPieceTexture(const char piece) {
    std::string pieceString = controller.pieceToTextureString(piece);
    std::string path = PieceCfg::TEXTURE_PATH + pieceString + ".png";
    pieceTextures[pieceString] = LoadTexture(path.c_str());
}