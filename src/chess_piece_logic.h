#pragma once

#include <string>

// This class is responsible for the definition of a logical chess piece
class ChessPiece {
public:
    enum class PieceType { 
        EMPTY,
        WHITE_KING, 
        WHITE_QUEEN,
        WHITE_ROOK, 
        WHITE_BISHOP, 
        WHITE_KNIGHT, 
        WHITE_PAWN,
        BLACK_KING,
        BLACK_QUEEN,
        BLACK_ROOK,
        BLACK_BISHOP,
        BLACK_KNIGHT,
        BLACK_PAWN
    };

    ChessPiece(); // Default constructor (empty piece)
    ChessPiece(PieceType);  // Actual piece constructor
    ~ChessPiece(); // Destructor
    
    // Copy semantics
    ChessPiece(const ChessPiece& other); // Copy constructor
    ChessPiece& operator=(const ChessPiece& other); // Copy assignment
    
    // Move semantics
    ChessPiece(ChessPiece&& other) noexcept; // Move constructor
    ChessPiece& operator=(ChessPiece&& other) noexcept; // Move assignment
    
    PieceType getType() const; // Get the type of the piece
    std::string pieceToString() const; // Convert piece to string
    bool isPlayable() const; // True if type is not NONE

private:
    PieceType type; // Type of the piece
};