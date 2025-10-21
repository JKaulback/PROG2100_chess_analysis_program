#pragma once

#include "chess_piece_logic.h"
#include <vector>
#include <array>

class ChessLogic {
public:
    ChessLogic();
    ~ChessLogic();

    void initializeBoard();
    ChessPiece getPieceAt(int rank, int file) const;
    void makeMove(int fromRank, int fromFile, int toRank, int toFile);

private:
    std::array<std::array<ChessPiece, 8>, 8> board; // Board representation
    std::vector<ChessPiece> capturedPieces; // Captured pieces
    void addPieces(ChessPiece::PieceColor color); // Helper to add pieces to the board
};