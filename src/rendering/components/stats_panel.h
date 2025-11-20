#pragma once

#include <raylib.h>
#include <array>
#include <vector>
#include "../../application/chess_analysis_program.h"

class ChessAnalysisProgram;

class StatsPanel {
public:
    StatsPanel(const ChessAnalysisProgram& controller);
    
    void draw() const;

private:
    const ChessAnalysisProgram& controller;

    void drawStat(const std::string text, const int statIndex) const;
    
    void drawCurrentPlayer(const int statIndex) const;
    void drawHalfMoveClock(const int statIndex) const;
};