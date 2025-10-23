#include "chess_analysis_program.h"
#include "chess_gui.h"
#include "chess_logic.h"

int main() {

    ChessLogic logic{};
    ChessGUI gui{logic}; // Pass logic reference to GUI
    ChessAnalysisProgram app{gui, logic};

    app.run();
    return 0;
}  