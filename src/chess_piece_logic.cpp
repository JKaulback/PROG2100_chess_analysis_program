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

ChessPiece::PieceType ChessPiece::getType() const { return this->type; }

ChessPiece::PieceColor ChessPiece::getColor() const { return this->color; }

std::string ChessPiece::pieceToString() const {
    std::string pieceString = this->color == ChessPiece::PieceColor::WHITE_PIECE ? "w" : "b";
    switch (this->type) {
        case ChessPiece::PieceType::KING: return pieceString + "k";
        case ChessPiece::PieceType::QUEEN: return pieceString + "q";
        case ChessPiece::PieceType::ROOK: return pieceString + "r";
        case ChessPiece::PieceType::BISHOP: return pieceString + "b";
        case ChessPiece::PieceType::KNIGHT: return pieceString + "n";
        case ChessPiece::PieceType::PAWN: return pieceString + "p";
        case ChessPiece::PieceType::NONE: return "None";
        default: return "Unknown";
    }
}

bool ChessPiece::isPlayable() const {
    if ( this->type != PieceType::NONE ) return true;
    return false;
}
