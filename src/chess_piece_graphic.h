#pragma once

#include <raylib.h>
#include "chess_piece_logic.h"

// This class is responsible for the graphical representation of a chess piece
class ChessPieceGraphic {
public:
    ChessPieceGraphic(const ChessPiece& piece, Vector2 position);
    ~ChessPieceGraphic();
    
    // Copy constructor and assignment (needed for std::vector)
    ChessPieceGraphic(const ChessPieceGraphic& other);
    ChessPieceGraphic& operator=(const ChessPieceGraphic& other);
    
    // Move constructor and assignment (for efficiency)
    ChessPieceGraphic(ChessPieceGraphic&& other) noexcept;
    ChessPieceGraphic& operator=(ChessPieceGraphic&& other) noexcept;

    // Draw the chess piece at its position
    void draw(float scale);
    
    // Getters
    Texture2D getTexture() const { return pieceTexture; }
    const ChessPiece& getPiece() const { return piece; }
    Rectangle getClickableArea() const { return pieceRect; }
    Vector2 getPosition() const { return position; }

    // Setters
    void setPosition(Vector2 newPosition);
    void setPiece(const ChessPiece& newPiece);


private:
    Texture2D pieceTexture; // Texture for the chess piece
    Rectangle pieceRect; // Rectangle for mouse interaction
    ChessPiece piece; // Associated chess piece
    Vector2 position; // Position on screen

    // Helpers
    void loadPieceTexture(); // Load texture based on piece type and color
    void updateClickableArea();

};