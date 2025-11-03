#pragma once

#include <raylib.h>
#include <string>
#include <vector>
#include <map>
#include "../config/config.h"

// Forward declarations to avoid circular dependencies
class ChessAnalysisProgram;
// This class is responsible for the graphical user interface of the chess program
class ChessGUI {
public:
    ChessGUI(const ChessAnalysisProgram& controller); // Constructor with controller reference
    ~ChessGUI(); // Destructor

    void draw() const; // Draw GUI elements - pure rendering
    
    // Public methods for controller to access
    Vector2 screenPosToBoardPos(Vector2) const; // Translate screen position to board position
    Vector2 boardPosToScreenPos(Vector2) const; // Translate board position to screen position
    float getSquareSize() const;
    float getPieceSize() const;

private:
    // GUI Display elements
    Texture2D boardTexture; // Texture for the chess board
    std::map<std::string, Texture2D> pieceTextures;
    const ChessAnalysisProgram& controller; // Reference to the controller (const for read-only access)
    
    // Helper methods (drag state moved to controller)
    void initPieceTextures(); // Initialize all piece textures
    void loadPieceTexture(const std::string& pieceString); // Load a single piece texture
    void drawChessPieces() const; // Draw the chess pieces based on their logical position
    void drawGameOverScreen() const; // Draw a game over screen to overlay UI
    void drawStats() const;
    void drawHalfMoveClock(const int statIndex) const; // Draw the half move clock
    void drawBoardState(const int statIndex) const; // Draw the FEN representation of the board state
};