#pragma once

#include <vector>
#include <map>
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
    void executeMove(const int srcRank, const int srcFile, const int destRank, const int destFile);
    std::string pieceToTextureString(const Piece piece) const;
    std::pair<int, int> getKingPosition(const Player player) const;
    
    // Temporary move methods for validation
    void makeTemporaryMove(const int srcRank, const int srcFile, const int destRank, const int destFile);
    void undoTemporaryMove(const int srcRank, const int srcFile, const int destRank, const int destFile, const Piece capturedPiece);

    // Utility methods for validator (read-only access to game state)
    bool isSquareEmpty(const int rank, const int file) const;
    bool isWhitePiece(const int rank, const int file) const;
    bool isBlackPiece(const int rank, const int file) const;
    bool areSameColorPieces(const int srcRank, const int srcFile, const int destRank, const int destFile) const;
    const std::vector<Piece>& getCapturedPieces() const;
    Player getPieceOwner(const Piece piece) const;
    std::string getCurrentPositionString() const; // Generates a string based on board state, player turn, castling rights, and en passant target square
    
    // Board state queries for advanced validation
    bool isValidSquare(const int rank, const int file) const;
    bool isPawn(const Piece piece) const; // Made public for validator use
    bool isKing(const Piece piece) const; // Helper for king detection
    
    // Turn management
    Player getCurrentPlayer() const;
    bool isPlayerTurn(const Player player) const;
    void switchTurn();
    
    // Castling rights queries
    bool canCastleKingside(const Player player) const;
    bool canCastleQueenside(const Player player) const;
    
    // En passant queries
    bool isEnPassantAvailable() const;
    bool isEnPassantTarget(const int rank, const int file) const;
    std::pair<int, int> getEnPassantTarget() const;
    std::pair<int, int> getEnPassantPawn() const;
    
    // Execute castling move
    void executeCastling(const int srcRank, const int srcFile, const int destRank, const int destFile);
    
    // Execute en passant move
    void executeEnPassant(const int srcRank, const int srcFile, const int destRank, const int destFile);

    // Execute pawn promotion move
    void executePromotion(const int srcRank, const int srcFile, const int destRank, const int destFile, Piece promoteTo = Piece::EMPTY);

    // Game ending accessor checks
    int getHalfmoveClock() const;

    // Threefold repetition check
    bool hasThreefoldRepetition() const;
private:
    // Game state variables
    std::array<std::array<Piece, BoardCfg::BOARD_DIMENSION>, BoardCfg::BOARD_DIMENSION> board; // Board representation
    std::vector<Piece> capturedPieces; // Captured pieces
    Player currentPlayer; // Whose turn it is (White goes first)
    int halfMoveClock; // Moves since last pawn move or last capture
    std::map<std::string, int> positionOccurrances;
    
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
    void initializePieces(); // Helper to add pieces to the board
    void updateCastlingRights(const int fromRank, const int fromFile, const int toRank, const int toFile); // Update castling rights when pieces move
    void updateEnPassantState(const int fromRank, const int fromFile, const int toRank, const int toFile); // Update en passant state after moves
    void clearEnPassantState(); // Clear en passant availability
    void recordCurrentPosition(); // Add current board position to map or iterate if it already exists
    std::string pieceToFENString(const Piece piece) const; // Generate a FEN string based on piece type
    std::string piecePositionToFENString() const; // Generate a FEN string for the piece on the board 
    std::string castlingRightsToFENString() const; // Generate a FEN string for castling rights
    std::string enPassantTargetToFENString() const; // Generate a FEN string for an en passant target (- if none)
};