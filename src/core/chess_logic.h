#pragma once

#include <vector>
#include <array>
#include <string>

// This class is responsible for chess game logic
class ChessLogic {
public:
    enum class Piece { 
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

    ChessLogic();
    ~ChessLogic();

    Piece getPieceAt(const int rank, const int file) const;
    void makeMove(const int fromRank, const int fromFile, const int toRank, const int toFile);
    void initializeBoard();
    std::string pieceToString(Piece piece) const;
    
private:
    // Game state variables
    std::array<std::array<Piece, 8>, 8> board; // Board representation
    std::vector<Piece> capturedPieces; // Captured pieces

    // Helpers
    void initPieces(); // Helper to add pieces to the board
    bool isValidMove(const int fromRank, const int fromFile, const int toRank, const int toFile) const;

};