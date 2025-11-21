#pragma once

#include <raylib.h>

class ChessAnalysisProgram;

/**
 * Handles rendering coordinate labels (files a-h and ranks 1-8) around the board
 */
class CoordinateRenderer {
public:
    CoordinateRenderer(const ChessAnalysisProgram& controller);
    
    void draw() const;

private:
    const ChessAnalysisProgram& controller;
    void drawCoordinates() const;
};