#pragma once

#include <array>
#include <string>
#include <vector>
#include "../chess_move.h"
#include "../../config/config.h"

namespace BoardCfg = Config::Board;

class ChessBoard {
    public:
        ChessBoard();
        
        // Board state accessor methods
        char getPieceAt(const int rank, const int file) const;
        char getPieceOwner(const int rank, const int file) const;
        char getPieceOwner(const char piece) const;
        std::pair<int, int> getKingPosition(const char player) const;
        std::vector<char> getCapturedPieces() const;
        std::vector<char> getWhiteCapturedPieces() const;
        std::vector<char> getBlackCapturedPieces() const;
        void setCapturedPieces(const std::vector<char>& whiteCaptured, const std::vector<char>& blackCaptured);
        bool isValidSquare(const int rank, const int file) const;
        
        // Board manipulation
        void setPieceAt(const int rank, const int file, const char piece);
        void clearBoard();
        void resetToStartingPosition();
        void executeBasicMove(const ChessMove& move);
        void executeCastling(const ChessMove& move);
        void executeEnPassant(const ChessMove& move);
        void executePromotion(const ChessMove& move, const char promoteTo = 'Q');

        // Temporary moves for validation
        void makeTemporaryMove(const ChessMove& move);
        void undoTemporaryMove(const ChessMove& move, const char capturedPiece);

        // Utility methods for validator (read-only)
        bool isSquareEmpty(const int rank, const int file) const;
        bool isWhitePiece(const int rank, const int file) const;
        bool areSameColorPieces(const ChessMove& move) const;
        bool isPawn(const char piece) const;
        bool isKing(const char piece) const;

        // Piece string for textures
        std::string getPieceTextureString(const char piece) const;

    private:
        std::array<std::array<char, BoardCfg::BOARD_DIMENSION>, BoardCfg::BOARD_DIMENSION> board;
        std::vector<char> whiteCapturedPieces;
        std::vector<char> blackCapturedPieces;
        
        // Helper functions
        void boardInit();
        void addToCapturedPieces(const char capturedPiece);
        bool isValidBoardPosition(const int rank, const int file) const;
        bool isValidPiece(const char piece) const;

};