#include "stats_panel.h"
#include "../../config/config.h"

namespace PieceCfg = Config::Pieces;
namespace StatsCfg = Config::GameStatistics;

StatsPanel::StatsPanel(const ChessAnalysisProgram& controller) :
    controller(controller) {}

void StatsPanel::draw() const {
    drawCurrentPlayer(0);
    drawHalfMoveClock(1);
}

void StatsPanel::drawStat(const std::string text, const int statIndex) const {
    DrawText(text.c_str(), StatsCfg::START_DRAW_X + 100,
        StatsCfg::START_DRAW_Y + (StatsCfg::DRAW_STEP_Y * statIndex),
        StatsCfg::STATS_FONT_SIZE_PX, StatsCfg::STATS_FONT_COLOR);
}

void StatsPanel::drawCurrentPlayer(const int statIndex) const {
    drawStat(TextFormat("CURRENT PLAYER: %c", controller.getCurrentPlayer()), statIndex);
}

void StatsPanel::drawHalfMoveClock(const int statIndex) const {
    drawStat(TextFormat("HALFMOVE CLOCK: %i", controller.getHalfmoveClock()), statIndex);
}
