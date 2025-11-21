#include "moves_comp.h"
#include "ui_renderer.h"
#include "../../config/config.h"

namespace MoveCFG = Config::MovesPanel;

MovesComp::MovesComp(const ChessAnalysisProgram& controller) :
    controller(controller) {}

void MovesComp::draw() const {
    drawDialogWindow();
}

void MovesComp::drawDialogWindow() const {
    Rectangle panelBounds = getDialogBounds();

    // Draw panel using UIRenderer
    UIRenderer::drawPanelBackground(panelBounds, UIRenderer::PanelStyle::Moves);
    UIRenderer::drawPanelBorder(panelBounds);
    UIRenderer::drawPanelShadowLeft(panelBounds, 8);

    // Draw content
    drawDialogTitle(panelBounds);
    drawMoves(panelBounds);
}

void MovesComp::drawDialogTitle(const Rectangle& panelBounds) const {
    UIRenderer::drawPanelTitle(panelBounds, MoveCFG::TITLE_TEXT,
                                MoveCFG::TITLE_HEIGHT, MoveCFG::PANEL_PADDING);
}

void MovesComp::drawMoves(const Rectangle& panelBounds) const {
    const std::vector<PositionState> positionHistory = controller.getPositionHistory();
    if (positionHistory.size() < 2) {
        std::string text = "No moves yet!";
        
        int textX = panelBounds.x + MoveCFG::PANEL_PADDING;
        int textY = panelBounds.y + MoveCFG::TITLE_HEIGHT + 8 + MoveCFG::PANEL_PADDING;

        UIRenderer::drawTextWithShadow(
            text, 
            textX, 
            textY, 
            18, 
            Color{128, 128, 128, 255});
        return;
    }

    std::string text = "Moves found!";
        
        int textX = panelBounds.x + MoveCFG::PANEL_PADDING;
        int textY = panelBounds.y + MoveCFG::TITLE_HEIGHT + 8 + MoveCFG::PANEL_PADDING;

        UIRenderer::drawTextWithShadow(text, textX, textY + MoveCFG::LINE_HEIGHT, 
                                        18, Color{128, 128, 128, 255});
}

void MovesComp::drawMove(const PositionState& moveData) const {

}

Rectangle MovesComp::getDialogBounds() const {
    // Position as a built-in panel on the right side, center of window
    float verticalCenterOffset = (Config::Window::HEIGHT - MoveCFG::PANEL_HEIGHT) / 2.0f;
    float horizontalOffset = Config::Window::WIDTH - MoveCFG::PANEL_WIDTH;
    return Rectangle{
        horizontalOffset,
        verticalCenterOffset,
        MoveCFG::PANEL_WIDTH,
        MoveCFG::PANEL_HEIGHT
    };
}

void MovesComp::drawText(const std::string& text, int x, int y, int fontSize, Color textColor) const {

}