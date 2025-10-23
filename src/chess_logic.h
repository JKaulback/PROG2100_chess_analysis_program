#pragma once

#include "chess_piece_logic.h"
#include <vector>
#include <array>

// This class is responsible for chess game logic
class ChessLogic {
public:
    ChessLogic();
    ~ChessLogic();

    ChessPiece getPieceAt(int rank, int file) const;
    void makeMove(int fromRank, int fromFile, int toRank, int toFile);
    void initializeBoard();
    
private:
    std::array<std::array<ChessPiece, 8>, 8> board; // Board representation
    std::vector<ChessPiece> capturedPieces; // Captured pieces
    void initPieces(ChessPiece::PieceColor color); // Helper to add pieces to the board
    
};