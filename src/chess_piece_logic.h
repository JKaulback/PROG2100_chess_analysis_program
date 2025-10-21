#pragma once

#include <string>

// Pure logic class - no GUI dependencies
class ChessPiece {
public:
    enum class PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, NONE };
    enum class PieceColor { WHITE_PIECE, BLACK_PIECE };

    ChessPiece(); // Default constructor (empty piece)
    ChessPiece(PieceType, PieceColor);  // Actual piece constructor
    ~ChessPiece(); // Destructor
    
    // Copy semantics
    ChessPiece(const ChessPiece& other); // Copy constructor
    ChessPiece& operator=(const ChessPiece& other); // Copy assignment
    
    // Move semantics
    ChessPiece(ChessPiece&& other) noexcept; // Move constructor
    ChessPiece& operator=(ChessPiece&& other) noexcept; // Move assignment
    
    PieceType getType() const; // Get the type of the piece
    PieceColor getColor() const; // Get the color of the piece
    
private:
    PieceType type; // Type of the piece
    PieceColor color; // Color of the piece
};