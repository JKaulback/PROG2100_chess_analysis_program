#include "chess_gui.h"


    ChessGUI::ChessGUI(ChessLogic& logic) : chessLogic(logic) {
        // Initialization
        const int screenWidth = 1920;
        const int screenHeight = 1080;

        InitWindow(screenWidth, screenHeight, "Chess Analysis Program");

        SetTargetFPS(60); // Set the desired frame rate

        // Load assets
        boardTexture = LoadTexture("src/assets/board.png"); // The chess board is initially 1600x1600 px

        // Initialize pieces from the logic board
        initChessPieces();
    }

    ChessGUI::~ChessGUI() {
        UnloadTexture(boardTexture);
        CloseWindow(); // Close window and OpenGL context
    }

    void ChessGUI::initChessPieces() {
        // Reset all graphical pieces
        piecesInPlay.clear();
        piecesCaptured.clear();

        for ( int rank = 0; rank < 8; rank++ ) {
            for ( int file = 0; file < 8; file++ ) {
                ChessPiece currentPiece = this->chessLogic.getPieceAt(rank, file);
                if (currentPiece.isPlayable()) {
                    float centeredValue = (squareSize / 2.0f) - (pieceSize / 2.0f);
                    Vector2 position{
                        boardOffsetX + (file * squareSize) + centeredValue,
                        boardOffsetY + ((7 - rank) * squareSize) + centeredValue
                    };
                    ChessPieceGraphic currentGraphic{currentPiece, std::move(position)};
                    piecesInPlay.push_back(currentGraphic);
                }
            }
        }
    }

    void ChessGUI::draw() {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw the chess board
        DrawTextureEx(this->boardTexture, {0, 0}, 0.0f, this->boardScale, WHITE);

        // Draw all chess pieces in play
        for (ChessPieceGraphic& pieceDrawing : piecesInPlay) {
            pieceDrawing.draw(this->pieceScale);
        }

        EndDrawing();
    }

    void ChessGUI::update() {
        // Update GUI elements if needed
    }


