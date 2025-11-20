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
    void drawTextWithBackground(const std::string& text, int x, int y, int fontSize, Color textColor, Color bgColor = Color{220, 230, 240, 200}) const;
    
    void drawCurrentPlayer(const int statIndex) const;
    void drawHalfMoveClock(const int statIndex) const;
    void drawGameStatus(const int statIndex) const;
    void drawCapturedPieces(const int statIndex) const;
};