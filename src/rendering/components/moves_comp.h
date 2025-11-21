#pragma once

#include <raylib.h>
#include "../../config/config.h"
#include "../../application/chess_analysis_program.h"

class ChessAnalysisProgram;

class MovesComp {
public:
    MovesComp(const ChessAnalysisProgram& controller);

    void draw() const;

private:
    const ChessAnalysisProgram& controller;


    void drawDialogWindow() const;
    void drawDialogTitle(const Rectangle& panelBounds) const;
    void drawMoves(const Rectangle& panelBounds) const;
    void drawMove(const PositionState& moveData) const;

    // Helper functions
    Rectangle getDialogBounds() const;
    void drawText(const std::string& text, int x, int y, int fontSize, Color textColor) const;
};