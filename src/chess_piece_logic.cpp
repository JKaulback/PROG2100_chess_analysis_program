#include "chess_piece_logic.h"

// Default constructor
ChessPiece::ChessPiece() 
    : type(PieceType::EMPTY) {}

// Unique piece constructor
ChessPiece::ChessPiece(PieceType type) 
    : type(type) {}

// Destructor
ChessPiece::~ChessPiece() {}

// Copy constructor
ChessPiece::ChessPiece(const ChessPiece& other) 
    : type(other.type) {}

// Copy assignment operator
ChessPiece& ChessPiece::operator=(const ChessPiece& other) {
    if (this != &other) { // Self-assignment check
        type = other.type;
    }
    return *this;
}

// Move constructor
ChessPiece::ChessPiece(ChessPiece&& other) noexcept 
    : type(other.type) {
    // Reset the moved-from object to a valid state
    other.type = PieceType::EMPTY;
}

// Move assignment operator
ChessPiece& ChessPiece::operator=(ChessPiece&& other) noexcept {
    if (this != &other) { // Self-assignment check
        type = other.type;
        
        // Reset the moved-from object to a valid state
        other.type = PieceType::EMPTY;
    }
    return *this;
}

ChessPiece::PieceType ChessPiece::getType() const { return this->type; }

std::string ChessPiece::pieceToString() const {
    switch (this->type) {
        case ChessPiece::PieceType::WHITE_KING: return "wk";
        case ChessPiece::PieceType::WHITE_QUEEN: return  "wq";
        case ChessPiece::PieceType::WHITE_ROOK: return  "wr";
        case ChessPiece::PieceType::WHITE_BISHOP: return "wb";
        case ChessPiece::PieceType::WHITE_KNIGHT: return "wn";
        case ChessPiece::PieceType::WHITE_PAWN: return "wp";
        case ChessPiece::PieceType::BLACK_KING: return "bk";
        case ChessPiece::PieceType::BLACK_QUEEN: return "bq";
        case ChessPiece::PieceType::BLACK_ROOK: return "br";
        case ChessPiece::PieceType::BLACK_BISHOP: return "bb";
        case ChessPiece::PieceType::BLACK_KNIGHT: return "bn";
        case ChessPiece::PieceType::BLACK_PAWN: return "bp";
        case ChessPiece::PieceType::EMPTY: return "none";
        default: return "Unknown";
    }
}

bool ChessPiece::isPlayable() const {
    return type != PieceType::EMPTY;
}
