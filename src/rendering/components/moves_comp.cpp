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

    int movesCount = 0;
    // The current set of moves (white then black) is either the last index of
    // positionHistory, or the second last index of positionHistory
    // (Note: If program gets here, positionHistory size >= 2)
    int currentSetStartIndex =
        (positionHistory.size() % 2 == 0 ) ?
        positionHistory.size() - 1 :
        positionHistory.size() - 1;

    for (int i = 1; i < positionHistory.size(); i++) {
        drawHistoricalMove(panelBounds, positionHistory.at(i), movesCount++, i >= currentSetStartIndex);
    }
}

void MovesComp::drawHistoricalMove(const Rectangle& panelBounds, const PositionState& moveData, const int movesCount, const bool isCurrentSet) const {
    // If white's move, add numeric identifier. Don't add for black
    std::string moveText =
        (moveData.movedBy == 'w') ?
        "" :
        std::to_string(movesCount / 2 + 1) + ". ";
    // Get the rest of the move text
    moveText += moveData.algebraicMove;

    // Determine x position of move in 6x8 grid (font size 18px) 
    int movePerCol = 6;
    int gridRows = 8;

    // textX and textY were handwritten to test my understanding
    int textX = 
        panelBounds.x + MoveCFG::PANEL_PADDING + // First index can start here 
        (movesCount % movePerCol) * // Index in the grid
        (MoveCFG::PANEL_WIDTH - (2 * MoveCFG::PANEL_PADDING)) /  
        movePerCol; // Break the drawable panel area into moves per col
    
    int textY = 
        panelBounds.y + MoveCFG::PANEL_PADDING + MoveCFG::TITLE_HEIGHT + // First index can start here
        (movesCount / movePerCol) * // Check index in the grid
        (MoveCFG::PANEL_HEIGHT - (2 * MoveCFG::PANEL_PADDING) - MoveCFG::TITLE_HEIGHT) / 
        gridRows; // Break the drawable panel area into number of rows


    // Determine color
    Color moveColor =
        isCurrentSet ?
        Color{0, 0, 139, 255} :
        Color{45, 45, 45, 255};
    // Draw the text
    UIRenderer::drawTextWithShadow(
            moveText, 
            textX, 
            textY, 
            12, 
            moveColor);
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
