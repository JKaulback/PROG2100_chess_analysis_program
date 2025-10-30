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
    std::pair<int, int> getKingPosition(Player player) const;
    
    // Temporary move methods for validation
    void makeTemporaryMove(int fromRank, int fromFile, int toRank, int toFile);
    void undoTemporaryMove(int fromRank, int fromFile, int toRank, int toFile, Piece capturedPiece);

    // Utility methods for validator (read-only access to game state)
    bool isSquareEmpty(const int rank, const int file) const;
    bool isWhitePiece(const int rank, const int file) const;
    bool isBlackPiece(const int rank, const int file) const;
    bool areSameColorPieces(const int rank1, const int file1, const int rank2, const int file2) const;
    const std::vector<Piece>& getCapturedPieces() const;
    
    // Board state queries for advanced validation
    bool isValidSquare(const int rank, const int file) const;
    bool isPawn(Piece piece) const; // Made public for validator use
    bool isKing(Piece piece) const; // Helper for king detection
    
    // Turn management
    Player getCurrentPlayer() const;
    bool isPlayerTurn(Player player) const;
    void switchTurn();
    
    // Castling rights queries
    bool canCastleKingside(Player player) const;
    bool canCastleQueenside(Player player) const;
    
    // En passant queries
    bool isEnPassantAvailable() const;
    bool isEnPassantTarget(int rank, int file) const;
    std::pair<int, int> getEnPassantTarget() const;
    std::pair<int, int> getEnPassantPawn() const;
    
    // Execute castling move
    void executeCastling(int fromRank, int fromFile, int toRank, int toFile);
    
    // Execute en passant move
    void executeEnPassant(int fromRank, int fromFile, int toRank, int toFile);

    // Execute pawn promotion move
    void executePromotion(int fromRank, int fromFile, int toRank, int toFile, Piece promoteTo = Piece::EMPTY);
private:
    // Game state variables
    std::array<std::array<Piece, BoardCfg::BOARD_DIMENSION>, BoardCfg::BOARD_DIMENSION> board; // Board representation
    std::vector<Piece> capturedPieces; // Captured pieces
    Player currentPlayer; // Whose turn it is (White goes first)
    
    // Castling rights tracking
    bool whiteKingMoved = false;
    bool blackKingMoved = false;
    bool whiteKingsideRookMoved = false;
    bool whiteQueensideRookMoved = false;
    bool blackKingsideRookMoved = false;
    bool blackQueensideRookMoved = false;
    
    // En passant tracking
    int enPassantTargetRank = -1;  // -1 means no en passant available
    int enPassantTargetFile = -1;  // The square where the capturing pawn will land
    int enPassantPawnRank = -1;    // The rank of the pawn that can be captured
    int enPassantPawnFile = -1;    // The file of the pawn that can be captured

    // Helpers
    void initPieces(); // Helper to add pieces to the board
    Player getPieceOwner(Piece piece) const; // Get which player owns a piece
    void updateCastlingRights(int fromRank, int fromFile); // Update castling rights when pieces move
    void updateEnPassantState(int fromRank, int fromFile, int toRank, int toFile); // Update en passant state after moves
    void clearEnPassantState(); // Clear en passant availability
};