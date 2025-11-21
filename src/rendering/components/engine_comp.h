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
    
    // Dialog window properties
    static constexpr int DIALOG_WIDTH = 450;
    static constexpr int DIALOG_HEIGHT = 350;
    static constexpr int DIALOG_PADDING = 20;
    static constexpr int LINE_HEIGHT = 28;
    static constexpr int TITLE_HEIGHT = 38;
    
    void drawDialogWindow() const;
    void drawDialogTitle() const;
    void drawEngineStatus() const;
    void drawEngineAnalysis() const;
    void drawAnalysisLine(const std::string& text, const int multipv, int& currentY) const;
    void drawEngineControls() const;
    
    // Helper functions
    Rectangle getDialogBounds() const;
    void drawText(const std::string& text, int x, int y, int fontSize, Color textColor) const;
};