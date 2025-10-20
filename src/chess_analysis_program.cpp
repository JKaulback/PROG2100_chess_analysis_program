#include <raylib.h>
#include "chess_analysis_program.h"

ChessAnalysisProgram::ChessAnalysisProgram() {
    // Initialization
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "Chess Analysis Program");

    SetTargetFPS(60); // Set the desired frame rate

    // Load assets
    boardTexture = LoadTexture("src/assets/board.png");

}

ChessAnalysisProgram::~ChessAnalysisProgram() {
    UnloadTexture(boardTexture);
    CloseWindow(); // Close window and OpenGL context
}

void ChessAnalysisProgram::UpdateGame(){
    // Update game state logic here
}

void ChessAnalysisProgram::RenderGame(){
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTextureEx(this->boardTexture, {0, 0}, 0.0f, 0.65f, WHITE);

        EndDrawing();
}

void ChessAnalysisProgram::run() {

    // Main program loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        UpdateGame();
        RenderGame();
    }

    // De-initialization
}