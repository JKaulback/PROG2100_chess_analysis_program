#pragma once

#include <raylib.h>
#include <map>
#include <string>

class ChessAnalysisProgram;

/**
 * Manages loading and unloading of textures for the chess board and pieces
 */
class TextureManager {
public:
    TextureManager(const ChessAnalysisProgram& controller);
    ~TextureManager();
    
    void loadTextures();
    bool areTexturesLoaded() const;
    
    const Texture2D& getBoardTexture() const;
    const Texture2D& getPieceTexture(const std::string& pieceString) const;

private:
    const ChessAnalysisProgram& controller;
    Texture2D boardTexture;
    std::map<std::string, Texture2D> pieceTextures;
    bool texturesLoaded;
    
    void loadPieceTexture(const char piece);
};