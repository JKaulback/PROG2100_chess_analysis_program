#include "chess_analysis_program.h"

int main() {

    ChessLogic logic{};
    ChessGUI gui{logic}; // Pass logic reference to GUI
    ChessAnalysisProgram app{gui};

    app.run();
    
    return 0;
}  