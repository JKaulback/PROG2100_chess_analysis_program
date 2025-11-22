#pragma once

#include <raylib.h>
#include <array>

namespace Config {
    // Window settings
    namespace Window {
        constexpr int WIDTH = 1920;
        constexpr int HEIGHT = 1080;
        constexpr int TARGET_FPS = 60;
        constexpr const char* TITLE = "Chess Analysis Program";
        constexpr float CENTER_X = WIDTH / 2.0f;
        constexpr float CENTER_Y = HEIGHT / 2.0f; 
    }

    // Board settings
    namespace Board {
        constexpr const char* TEXTURE_PATH = "src/assets/board.png";
        constexpr float SCALE = 0.5f;  // Reduced from 0.675 to 0.5
        constexpr float TEXTURE_SIZE = 1600.0f;
        constexpr float SIZE = TEXTURE_SIZE * SCALE;
        constexpr float SQUARE_SIZE = SIZE / 8.0f;
        constexpr float COORDINATE_MARGIN = 40.0f;  // Space for coordinate labels
        constexpr float AESTHETIC_MARGIN = 30.0f;   // Additional aesthetic spacing
        constexpr float TOTAL_MARGIN = COORDINATE_MARGIN + AESTHETIC_MARGIN;
        constexpr float PANEL_SPACE = 450.0f;      // Space for left-side panels
        constexpr float BOARD_WITH_MARGINS = SIZE + (2 * TOTAL_MARGIN);  // Board + margins
        constexpr float VERTICAL_CENTER_OFFSET = (Window::HEIGHT - BOARD_WITH_MARGINS) / 2.0f;
        constexpr float OFFSET_X = PANEL_SPACE + TOTAL_MARGIN;  // Move board to right
        constexpr float OFFSET_Y = VERTICAL_CENTER_OFFSET + TOTAL_MARGIN;
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
    
        // Valid board values
        constexpr char EMPTY = '-';
        constexpr std::array<char, 12> VALID_PIECES = {
            'P', 'R', 'N', 'B', 'Q', 'K', // White pieces
            'p', 'r', 'n', 'b', 'q', 'k' // Black pieces
        };
    }

    // Piece settings
    namespace Pieces {
        constexpr const char* TEXTURE_PATH = "src/assets/chess_pieces/";
        constexpr float SIZE_RATIO = 0.8f;
        constexpr float SIZE = Board::SQUARE_SIZE * SIZE_RATIO;
        constexpr float TEXTURE_SIZE = 150.0f;
        constexpr float SCALE = SIZE / TEXTURE_SIZE;
        constexpr float CENTER_OFFSET = (Board::SQUARE_SIZE / 2.0f) - (SIZE / 2.0f);
        constexpr float CAPTURED_SIZE_RATIO = 0.4f;
        constexpr float CAPTURED_SIZE = Board::SQUARE_SIZE * CAPTURED_SIZE_RATIO;
        constexpr float CAPTURED_SCALE = CAPTURED_SIZE / TEXTURE_SIZE;
        constexpr float CAPTURED_OFFSET_X = Board::SIZE + Board::OFFSET_X + 50.0f;  // Position right of board
        constexpr float CAPTURED_MARGIN = 50.0f;
        constexpr float CAPTURED_OFFSET_Y_WHITE = Board::OFFSET_Y + CAPTURED_MARGIN;
        constexpr float CAPTURED_OFFSET_Y_BLACK = Board::SIZE + Board::OFFSET_Y - CAPTURED_MARGIN - CAPTURED_SIZE;
        constexpr float CAPTURED_STEP = CAPTURED_SIZE * 0.9f;
        constexpr int MAX_CAPTURED_IN_ROW = 7;
    }

    // Game Over settings
    namespace GameOver {
        constexpr const char* DRAW_50_MOVES_STRING = "DRAW: 50 MOVE RULE";
        constexpr const char* DRAW_THREEFOLD_REPETITION_STRING = "DRAW:\nTHREEFOLD REPETITION";
        constexpr const char* DRAW_INSUFFICIENT_MATERIAL_STRING = "DRAW:\nINSUFFICIENT MATERIAL";
        constexpr const char* STALEMATE_STRING = "STALEMATE";
        constexpr const char* WHITE_WIN_STRING = "WHITE WINS!";
        constexpr const char* BLACK_WIN_STRING = "BLACK WINS!";
        constexpr const char* ERROR_STRING = "ERROR!!!";
        constexpr int STATE_FONT_SIZE_PX = 100;
        constexpr int STATE_FONT_SPACING = 1;
        constexpr Color STATE_FONT_COLOR = RED;
        constexpr int BACKGROUND_POS_X = 0;
        constexpr int BACKGROUND_POS_Y = 0;
        constexpr Color BACKGROUND_COLOR = {100, 100, 100, 128};
    }

    namespace GameStatistics {
        constexpr int STATS_FONT_SIZE_PX = 20;
        constexpr int STATS_FONT_SPACING = 1;
        constexpr Color STATS_FONT_COLOR = BLACK;

        // Position stats panel below engine analysis to avoid overlap
        constexpr int START_DRAW_X = Window::WIDTH - 400; // Same X as engine for alignment
        constexpr int START_DRAW_Y = Window::HEIGHT / 2; // Position lower on screen
        constexpr int DRAW_STEP_Y = STATS_FONT_SIZE_PX + 12; // Single spaced
    }

    namespace Controls {
        constexpr int FONT_SIZE_PX = 16;
        constexpr int FONT_SPACING = 1;
        constexpr Color FONT_COLOR = BLACK;
        
        // Position engine analysis further right to avoid captured pieces
        constexpr int START_DRAW_X = Window::WIDTH - 400; // More space for captured pieces
        constexpr int START_DRAW_Y = Window::HEIGHT / 8;
        constexpr int DRAW_STEP_Y = FONT_SIZE_PX;
        
    }

    // UI Component Configuration
    namespace EngineDialog {
        constexpr int DIALOG_WIDTH = 450;
        constexpr int DIALOG_HEIGHT = 350;
        constexpr int DIALOG_PADDING = 20;
        constexpr int LINE_HEIGHT = 28;
        constexpr int TITLE_HEIGHT = 38;
        constexpr const char* TITLE_TEXT = "ENGINE ANALYSIS";
    }

    namespace StatsPanel {
        constexpr int PANEL_WIDTH = 450;
        constexpr int PANEL_HEIGHT = 200;
        constexpr int PANEL_PADDING = 20;
        constexpr int LINE_HEIGHT = 24;
        constexpr int TITLE_HEIGHT = 36;
        constexpr const char* TITLE_TEXT = "GAME STATISTICS";
    }

    namespace ControlsPanel {
        constexpr int PANEL_WIDTH = 450;
        constexpr int PANEL_HEIGHT = 340;
        constexpr int PANEL_PADDING = 20;
        constexpr int LINE_HEIGHT = 22;
        constexpr int TITLE_HEIGHT = 36;
        constexpr const char* TITLE_TEXT = "CONTROLS";
    }

    namespace Coordinates {
        constexpr int FONT_SIZE = 24;
        constexpr float TOP_OFFSET = 35.0f;
        constexpr float LEFT_OFFSET = 35.0f;
        constexpr Color LABEL_COLOR = {220, 220, 220, 255};  // Light gray for high contrast
        constexpr Color SHADOW_COLOR = {20, 25, 35, 200};    // Dark shadow matching background
        constexpr Color OUTLINE_COLOR = {40, 50, 65, 255};   // Dark outline for definition
    }

    namespace CapturedPieces {
        constexpr int LABEL_FONT_SIZE = 16;
        constexpr float LABEL_OFFSET = 15.0f;
        constexpr Color LABEL_COLOR = {139, 69, 19, 255}; // Same brown as border
        constexpr Color WHITE_BACKGROUND = {240, 228, 210, 255};  // Light cream for white pieces
        constexpr Color BLACK_BACKGROUND = {139, 114, 85, 255};   // Darker brown for black pieces
    }

    // Visual styling constants
    namespace Borders {
        constexpr float BOARD_BORDER_WIDTH = 8.0f;
        constexpr float CAPTURED_BORDER_WIDTH = 6.0f;
        constexpr float SHADOW_OFFSET_BOARD = 6.0f;
        constexpr float SHADOW_OFFSET_CAPTURED = 4.0f;
        constexpr float ACCENT_WIDTH_BOARD = 2.0f;
        constexpr float ACCENT_WIDTH_CAPTURED = 1.5f;
        constexpr float HIGHLIGHT_WIDTH = 1.0f;
        
        // Colors
        constexpr Color BORDER_COLOR = {139, 69, 19, 255};      // Saddle brown - elegant wood color
        constexpr Color INNER_BORDER = {160, 82, 45, 255};      // Lighter brown accent
        constexpr Color SHADOW_COLOR = {0, 0, 0, 60};           // Subtle shadow
        constexpr Color HIGHLIGHT_COLOR = {205, 133, 63, 255};  // Peru - lighter highlight
    }

    namespace Decorations {
        constexpr float CORNER_SIZE_BOARD = 16.0f;
        constexpr float CORNER_SIZE_CAPTURED = 12.0f;
        constexpr float CORNER_OFFSET_BOARD = 4.0f;
        constexpr float CORNER_OFFSET_CAPTURED = 3.0f;
        constexpr float CORNER_INSET_BOARD = 12.0f;
        constexpr float CORNER_INSET_CAPTURED = 9.0f;
        constexpr Color DECOR_COLOR = {205, 133, 63, 180};  // Semi-transparent peru
        constexpr Color ACCENT_COLOR = {139, 69, 19, 255};  // Saddle brown
    }

    namespace MovesPanel {
        constexpr int PANEL_WIDTH = 550;
        constexpr int PANEL_HEIGHT = 450;
        constexpr int PANEL_PADDING = 20;
        constexpr int LINE_HEIGHT = 22;
        constexpr int TITLE_HEIGHT = 36;
        constexpr const char* TITLE_TEXT = "MOVE HISTORY";
        constexpr int MOVE_FONT_SIZE = 18;
    }

    namespace Fonts {
        // Font file paths (relative to executable or absolute paths)
        constexpr const char* MONOSPACE_FONT_PATH = "assets/fonts/monospace.ttf";
        constexpr const char* FALLBACK_MONOSPACE_PATH = "C:/Windows/Fonts/consola.ttf"; // Windows Consolas
        constexpr const char* FALLBACK_MONOSPACE_PATH_2 = "C:/Windows/Fonts/cour.ttf";   // Windows Courier New
        
        // Font sizes for different UI components
        constexpr int BASE_FONT_SIZE = 48;      // Base size for font loading (higher quality)
        constexpr int DEFAULT_UI_SIZE = 16;     // Default UI text size
        constexpr int MONOSPACE_UI_SIZE = 14;   // Monospace text size
        constexpr int TITLE_SIZE = 22;          // Panel titles
        constexpr int SMALL_SIZE = 12;          // Small text
        
        // Font spacing
        constexpr float MONOSPACE_SPACING = 1.0f;
        constexpr float DEFAULT_SPACING = 1.0f;
    }
}