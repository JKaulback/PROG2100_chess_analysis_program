#pragma once

namespace Config {
    // Window settings
    namespace Window {
        constexpr int WIDTH = 1920;
        constexpr int HEIGHT = 1080;
        constexpr int TARGET_FPS = 60;
        constexpr const char* TITLE = "Chess Analysis Program";
    }

    // Board settings
    namespace Board {
        constexpr const char* TEXTURE_PATH = "src/assets/board.png";
        constexpr float SCALE = 0.675f;
        constexpr float TEXTURE_SIZE = 1600.0f;
        constexpr float SIZE = TEXTURE_SIZE * SCALE;
        constexpr float SQUARE_SIZE = SIZE / 8.0f;
        constexpr float OFFSET_X = 0.0f;
        constexpr float OFFSET_Y = 0.0f;
        constexpr int MIN_RANK = 0;
        constexpr int MAX_RANK = 7;
        constexpr int MIN_FILE = 0;
        constexpr int MAX_FILE = 7;
        constexpr int BOARD_DIMENSION = 8;
        
        // Chess-specific constants
        constexpr int WHITE_PAWN_START_RANK = 1;
        constexpr int BLACK_PAWN_START_RANK = 6;
        constexpr int WHITE_BACK_RANK = 0;
        constexpr int BLACK_BACK_RANK = 7;
        constexpr int KING_FILE = 4;
        constexpr int QUEENSIDE_ROOK_FILE = 0;
        constexpr int KINGSIDE_ROOK_FILE = 7;
        constexpr int QUEENSIDE_CASTLE_KING_FILE = 2;
        constexpr int KINGSIDE_CASTLE_KING_FILE = 6;
        constexpr int QUEENSIDE_CASTLE_ROOK_FILE = 3;
        constexpr int KINGSIDE_CASTLE_ROOK_FILE = 5;
        
        // Promotion ranks (where pawns promote)
        constexpr int WHITE_PROMOTES_AT_RANK = BLACK_BACK_RANK;  // White promotes at rank 7
        constexpr int BLACK_PROMOTES_AT_RANK = WHITE_BACK_RANK;  // Black promotes at rank 0
        
        // Pawn movement directions
        constexpr int WHITE_PAWN_DIRECTION = 1;   // White pawns move up (positive direction)
        constexpr int BLACK_PAWN_DIRECTION = -1;  // Black pawns move down (negative direction)
        
        // En passant capture ranks
        constexpr int WHITE_EN_PASSANT_CAPTURE_RANK = 4;  // White captures en passant from rank 4
        constexpr int BLACK_EN_PASSANT_CAPTURE_RANK = 3;  // Black captures en passant from rank 3
    }

    // Piece settings
    namespace Pieces {
        constexpr const char* TEXTURE_PATH = "src/assets/chess_pieces/";
        constexpr float SIZE_RATIO = 0.8f;
        constexpr float SIZE = Board::SQUARE_SIZE * SIZE_RATIO;
        constexpr float TEXTURE_SIZE = 150.0f;
        constexpr float SCALE = SIZE / TEXTURE_SIZE;
        constexpr float CENTER_OFFSET = (Board::SQUARE_SIZE / 2.0f) - (SIZE / 2.0f);
    }
}