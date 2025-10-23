#pragma once

#include "chess_gui.h"

// This class manages the overall chess analysis program
class ChessAnalysisProgram {

public:
    ChessAnalysisProgram(ChessGUI& gui); // Constructor
    ~ChessAnalysisProgram(); // Destructor

    void run(); // Main loop

private:
    ChessGUI& gui; // Reference to the GUI
};