#include "chess_piece_logic.h"

ChessPiece::ChessPiece() {
    type = PieceType::NONE; // Default type
    color = PieceColor::WHITE_PIECE; // Default color
}

ChessPiece::ChessPiece(PieceType type, PieceColor color) 
    : type(type), color(color) {}

ChessPiece::~ChessPiece() {}

// Copy constructor
ChessPiece::ChessPiece(const ChessPiece& other) 
    : type(other.type), color(other.color) {}

// Copy assignment operator
ChessPiece& ChessPiece::operator=(const ChessPiece& other) {
    if (this != &other) { // Self-assignment check
        type = other.type;
        color = other.color;
    }
    return *this;
}

// Move constructor
ChessPiece::ChessPiece(ChessPiece&& other) noexcept 
    : type(other.type), color(other.color) {
    // Reset the moved-from object to a valid state
    other.type = PieceType::NONE;
    other.color = PieceColor::WHITE_PIECE;
}

// Move assignment operator
ChessPiece& ChessPiece::operator=(ChessPiece&& other) noexcept {
    if (this != &other) { // Self-assignment check
        type = other.type;
        color = other.color;
        
        // Reset the moved-from object to a valid state
        other.type = PieceType::NONE;
        other.color = PieceColor::WHITE_PIECE;
    }
    return *this;
}

ChessPiece::PieceType ChessPiece::getType() const { return type; }

ChessPiece::PieceColor ChessPiece::getColor() const { return color; }
