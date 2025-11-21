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

    // Panel properties
    static constexpr int PANEL_WIDTH = 450;
    static constexpr int PANEL_HEIGHT = 200;
    static constexpr int PANEL_PADDING = 20;
    static constexpr int LINE_HEIGHT = 24;
    static constexpr int TITLE_HEIGHT = 36;
    
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