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