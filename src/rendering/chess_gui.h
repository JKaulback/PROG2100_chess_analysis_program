#pragma once

#include <raylib.h>
#include <memory>
#include "components/board_comp.h"
#include "components/controls_comp.h"
#include "components/engine_comp.h"
#include "components/game_overlay.h"
#include "components/moves_comp.h"
#include "components/stats_panel.h"
#include "../config/config.h"

class ChessAnalysisProgram;
class BoardComp;
class ControlsComp;
class EngineComp;
class GameOverlay;
class MovesComp;
class StatsPanel;

class ChessGUI {
public:
    ChessGUI(const ChessAnalysisProgram& controller);
    ~ChessGUI();

    void draw() const;
    
    // Board interaction methods
    Vector2 screenPosToBoardPos(const Vector2 pos) const;
    Vector2 boardPosToScreenPos(const Vector2 pos) const;
    float getSquareSize() const;
    float getPieceSize() const;

    // Engine state updates
    void setIsUCIEngineRunning(const bool isRunning);

private:
    // Background rendering
    void drawModernBackground() const;
    void drawGeometricPattern() const;
    void drawAmbientLighting() const;
    const ChessAnalysisProgram& controller;
    
    // GUI Components
    std::unique_ptr<BoardComp> boardComp;
    std::unique_ptr<ControlsComp> controlsComp;
    std::unique_ptr<EngineComp> engineComp;
    std::unique_ptr<GameOverlay> gameOverlay;
    std::unique_ptr<MovesComp> movesComp;
    std::unique_ptr<StatsPanel> statsPanel;
};