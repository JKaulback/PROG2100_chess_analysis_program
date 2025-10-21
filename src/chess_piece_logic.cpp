#include "chess_piece_logic.h"

ChessPiece::ChessPiece(PieceType type, PieceColor color, int rank, int file) 
    : type(type), color(color), rank(rank), file(file) {}

ChessPiece::~ChessPiece() {}

ChessPiece::PieceType ChessPiece::getType() const { return type; }

ChessPiece::PieceColor ChessPiece::getColor() const { return color; }

int ChessPiece::getRank() const { return rank; }

int ChessPiece::getFile() const { return file; }

void ChessPiece::setPosition(int newRank, int newFile) {
    this->rank = newRank;
    this->file = newFile;
}

bool ChessPiece::isValidMove(int toRank, int toFile) const {
    // Basic bounds checking - extend with piece-specific logic later
    return (toRank >= 0 && toRank < 8 && toFile >= 0 && toFile < 8);
}