#include "chess_analysis_program.h"

ChessAnalysisProgram::ChessAnalysisProgram(ChessGUI& gui) : gui(gui) {}

ChessAnalysisProgram::~ChessAnalysisProgram() {}

void ChessAnalysisProgram::run() {

    // Main loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        
        gui.update();
        gui.draw();
    }

    // De-initialization
}