#pragma once

#include <string>

// Pure logic class - no GUI dependencies
class ChessPiece {
public:
    enum class PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };
    enum class PieceColor { WHITE_PIECE, BLACK_PIECE };

    ChessPiece(PieceType, PieceColor, int, int); 
    ~ChessPiece();
    
    PieceType getType() const;
    PieceColor getColor() const;
    int getRank() const;
    int getFile() const;

    // Logic operations
    void setPosition(int newRank, int newFile);
    bool isValidMove(int toRank, int toFile) const;

private:
    PieceType type;
    PieceColor color;
    int rank, file; // Board coordinates (0-7)
};