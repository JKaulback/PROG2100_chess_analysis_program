#pragma once

#include <vector>
#include <array>
#include <string>
#include "../config/config.h"

namespace BoardCfg = Config::Board;

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

    enum class Player {
        WHITE_PLAYER,
        BLACK_PLAYER
    };

    ChessLogic();
    ~ChessLogic();

    // Core state access methods
    Piece getPieceAt(const int rank, const int file) const;
    void executeMove(const int fromRank, const int fromFile, const int toRank, const int toFile);
    void initializeBoard();
    std::string pieceToString(Piece piece) const;
    
    // Utility methods for validator (read-only access to game state)
    bool isSquareEmpty(const int rank, const int file) const;
    bool isWhitePiece(const int rank, const int file) const;
    bool isBlackPiece(const int rank, const int file) const;
    bool areSameColorPieces(const int rank1, const int file1, const int rank2, const int file2) const;
    const std::vector<Piece>& getCapturedPieces() const;
    
    // Board state queries for advanced validation
    bool isValidSquare(const int rank, const int file) const;
    
    // Turn management
    Player getCurrentPlayer() const;
    bool isPlayerTurn(Player player) const;
    void switchTurn();
private:
    // Game state variables
    std::array<std::array<Piece, BoardCfg::BOARD_DIMENSION>, BoardCfg::BOARD_DIMENSION> board; // Board representation
    std::vector<Piece> capturedPieces; // Captured pieces
    Player currentPlayer; // Whose turn it is (White goes first)

    // Helpers
    void initPieces(); // Helper to add pieces to the board
    Player getPieceOwner(Piece piece) const; // Get which player owns a piece
};