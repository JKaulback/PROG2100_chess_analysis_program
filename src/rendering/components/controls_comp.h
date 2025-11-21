#pragma once

#include <raylib.h>
#include <vector>
#include <string>
#include "../../config/config.h"
#include "../../application/chess_analysis_program.h"

class ChessAnalysisProgram;

class ControlsComp {
public:
    ControlsComp(const ChessAnalysisProgram& controller);
    
    void draw() const;

private:
    const ChessAnalysisProgram& controller;
    
    void drawControlsPanel() const;
    void drawPanelTitle() const;
    void drawControlGroup(const std::string& title, const std::vector<std::string>& controls, int& currentY) const;
    void drawControlItem(const std::string& text, int& currentY) const;
    void drawText(const std::string& text, int x, int y, int fontSize, Color textColor) const;
    
    Rectangle getPanelBounds() const;
};