#pragma once

#include <raylib.h>
#include <array>
#include <vector>
#include "../../config/config.h"
#include "../../application/chess_analysis_program.h"

class ChessAnalysisProgram;

class StatsPanel {
public:
    StatsPanel(const ChessAnalysisProgram& controller);
    
    void draw() const;

private:
    const ChessAnalysisProgram& controller;

    // Use Config::StatsPanel constants
    
    void drawStatsPanel() const;
    void drawPanelTitle() const;
    void drawStat(const std::string& label, const std::string& value, int& currentY) const;
    void drawText(const std::string& text, int x, int y, int fontSize, Color textColor) const;
    
    Rectangle getPanelBounds() const;
    void drawCurrentPlayer(int& currentY) const;
    void drawHalfMoveClock(int& currentY) const;
    void drawGameStatus(int& currentY) const;
    void drawCapturedPieces(int& currentY) const;
};