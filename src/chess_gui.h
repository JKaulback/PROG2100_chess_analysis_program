#pragma once

#include <raylib.h>
#include <map>
#include "chess_logic.h"

// This class is responsible for the graphical user interface of the chess program
class ChessGUI {

public:
    ChessGUI(ChessLogic& logic); // Constructor with logic reference
    ~ChessGUI(); // Destructor

    void draw() const; // Draw GUI elements
    void update(); // Update GUI elements
    
private:
    // Chess board and piece display
    Texture2D boardTexture; // Texture for the chess board
    std::map<std::string, Texture2D> pieceTextures;
    ChessLogic& chessLogic; // Reference to the chess logic
    
    // Program data
    const int SCREEN_WIDTH = 1920;
    const int SCREEN_HEIGHT = 1080;
    const int TARGET_FPS = 60;

    // Board texture data
    const char* BOARD_TEXTURE_PATH = "src/assets/board.png";
    const float BOARD_SCALE = 0.65f; // The scale to draw the board
    const float ACTUAL_BOARD_SIZE = 1600.0f * BOARD_SCALE; // Actual size of the board in px
    const float SQUARE_SIZE = ACTUAL_BOARD_SIZE / 8.0f; // Size of the squares of the board
    const float BOARD_OFFSET_X = 0.0f;
    const float BOARD_OFFSET_Y = 0.0f;
    const int MIN_RANK = 0;
    const int MIN_FILE = 0;
    const int MAX_RANK = 7;
    const int MAX_FILE = 7;

    // Piece texture data
    const char* PIECES_TEXTURE_PATH = "src/assets/chess_pieces/";
    const float PIECE_SIZE_RATIO = 0.8f;
    const float PIECE_SIZE = SQUARE_SIZE * PIECE_SIZE_RATIO; // Size of the pieces to draw
    const float PIECE_TEXTURE_SIZE = 150.0f;
    const float PIECE_SCALE = PIECE_SIZE / PIECE_TEXTURE_SIZE; // Scale of the pieces
    const float CENTERED_VALUE = (SQUARE_SIZE / 2.0f) - (PIECE_SIZE / 2.0f); // Value to add to piece position for centering
    
    // Drag and drop state
    bool isDragging; // Whether a piece is being dragged
    int draggedPieceRank; // The original rank of a dragged piece
    int draggedPieceFile; // The original file of a dragged piece
    Vector2 dragOffset; // The offset from the mouse to prevent "jump"
    ChessPiece draggedPiece; // The actual piece being dragged

    // Helper methods
    Vector2 screenPosToBoardPos(Vector2) const; // Translate screen position to board position
    Vector2 boardPosToScreenPos(Vector2) const; // Translate board position to screen position
    void initPieceTextures(); // Initialize all piece textures
    void loadPieceTexture(const std::string& pieceString); // Load a single piece texture
    void drawChessPieces() const; // Draw the chess pieces based on their logical position
    void resetDragState();
    
};