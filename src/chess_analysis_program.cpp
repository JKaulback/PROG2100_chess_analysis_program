#include "chess_analysis_program.h"

ChessAnalysisProgram::ChessAnalysisProgram(ChessGUI& gui, ChessLogic& logic) : gui(gui), logic(logic) {



}

ChessAnalysisProgram::~ChessAnalysisProgram() {
    // Cleanup if needed
}

void ChessAnalysisProgram::run() {

    // Main loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        
        gui.update();
        gui.draw();
    }

    // De-initialization
}