#pragma once

#include <raylib.h>
#include "../../application/chess_analysis_program.h"

class ChessAnalysisProgram;

class GameOverlay {
public:
    GameOverlay(const ChessAnalysisProgram& controller);
    
    void draw() const;

private:
    const ChessAnalysisProgram& controller;
    
    void drawGameOverScreen() const;
    void drawControls() const;
};