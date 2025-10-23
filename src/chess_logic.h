#pragma once

#include "chess_piece_logic.h"
#include <vector>
#include <array>

// This class is responsible for chess game logic
class ChessLogic {
public:
    ChessLogic();
    ~ChessLogic();

    ChessPiece getPieceAt(const int rank, const int file) const;
    void makeMove(const int fromRank, const int fromFile, const int toRank, const int toFile);
    void initializeBoard();
    
private:
    // Game state variables
    std::array<std::array<ChessPiece, 8>, 8> board; // Board representation
    std::vector<ChessPiece> capturedPieces; // Captured pieces

    // Helpers
    void initPieces(ChessPiece::PieceColor color); // Helper to add pieces to the board
    bool isValidMove(const int fromRank, const int fromFile, const int toRank, const int toFile) const;

};