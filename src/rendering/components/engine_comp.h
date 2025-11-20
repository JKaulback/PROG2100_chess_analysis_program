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
    void drawEngineControls() const;
};