#pragma once

#include "chess_gui.h"
#include "chess_logic.h"

// This class manages the overall chess analysis program
class ChessAnalysisProgram {

public:
    ChessAnalysisProgram(ChessGUI& gui, ChessLogic& logic); // Constructor
    ~ChessAnalysisProgram(); // Destructor

    void run(); // Main loop

private:
    ChessGUI& gui; // Reference to the GUI
    ChessLogic& logic; // Reference to the game logic
};