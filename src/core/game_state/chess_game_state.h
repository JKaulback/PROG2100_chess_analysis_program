#pragma once

#include <array>
#include <map>
#include <string>
#include "../chess_move.h"
#include "../board/chess_board.h"

class ChessGameState {
public:
    ChessGameState(const ChessBoard& board);

    // Update game state before move is made
    void makeMove(const ChessMove& move);

    // Player state
    char getCurrentPlayer() const;
    bool isPlayerTurn(const char player) const;

    // Castling right states
    bool canCastleKingside(const char player) const;
    bool canCastleQueenside(const char player) const;

    // En passant states
    bool isEnPassantAvailable() const;
    bool isEnPassantTarget(const int rank, const int file) const;
    std::pair<int, int> getEnPassantTarget() const;
    std::pair<int, int> getEnPassantPawn() const;

    // Game clock accessors
    int getHalfmoveClock() const;
    int getFullmoveClock() const;

    // Threefold repetition state
    bool hasThreefoldRepetition() const;
    
    // FEN loading support methods
    void setCurrentPlayer(const char player);
    void setCastlingRights(bool whiteKingside, bool whiteQueenside, bool blackKingside, bool blackQueenside);
    void setEnPassantTarget(const int rank, const int file);
    void clearEnPassantState();
    void setHalfmoveClock(const int halfmoves);
    void setFullmoveClock(const int fullmoves);
private:
    // Game state variables
    const ChessBoard* board;
    char currentPlayer;
    int fullmoveClock, halfmoveClock;
    
    // Castling rights tracking
    bool whiteKingMoved = false, blackKingMoved = false;
    bool whiteKRookMoved = false, whiteQRookMoved = false;
    bool blackKRookMoved = false, blackQRookMoved = false;

    // En passant tracking
    int enPassantTargetRank, enPassantTargetFile;
    int enPassantPawnRank, enPassantPawnFile;

    // Updating states
    void updateCastlingRights(const ChessMove& move, const char srcPiece, const char destPiece);
    void updateEnPassantState(const ChessMove& move, const char srcPiece);
    void resetHalfmoveClock();
    void resetCastlingRights();
};