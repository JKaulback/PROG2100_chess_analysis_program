#pragma once

#include <raylib.h>
#include "../../application/chess_analysis_program.h"

class ChessAnalysisProgram;

class EngineComp {
public:
    EngineComp(const ChessAnalysisProgram& controller);
    
    void draw() const;
    void setEngineRunning(bool isRunning);

private:
    const ChessAnalysisProgram& controller;
    bool isEngineRunning;
    
    void drawEngineStatus() const;
    void drawEngineAnalysis() const;
    void drawAnalysisLine(const std::string text, const int multipv) const;
    void drawEngineControls() const;
    
    // Helper function for drawing text with gray background
    void drawTextWithBackground(const std::string& text, int x, int y, int fontSize, Color textColor, Color bgColor = LIGHTGRAY) const;
};