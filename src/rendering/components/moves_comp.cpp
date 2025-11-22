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
    const std::vector<PositionState> positionRedos = controller.getRedoPositions();
    if (positionHistory.size() < 2 && positionRedos.empty()) {
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
    if (!positionRedos.empty()) {
        for (int i = positionRedos.size() - 1; i >= 0; i--) {
            drawRedoMove(panelBounds, positionRedos.at(i), movesCount++);
        }
    }
}

void MovesComp::drawHistoricalMove(const Rectangle& panelBounds, const PositionState& moveData, const int movesCount, const bool isCurrentSet) const {
    std::string moveText = getMoveText(moveData, movesCount);

    // Calculate x and y positions
    Vector2 textPosition = calcMoveTextPos(panelBounds, moveText, movesCount, MoveCFG::MOVE_FONT_SIZE);

    // Determine color
    Color moveColor =
        isCurrentSet ?
        Color{0, 0, 139, 255} :
        Color{45, 45, 45, 255};
    // Draw the text
    UIRenderer::drawTextWithShadow(
            moveText, 
            static_cast<int>(textPosition.x), 
            static_cast<int>(textPosition.y), 
            MoveCFG::MOVE_FONT_SIZE, 
            moveColor);
}

void MovesComp::drawRedoMove(const Rectangle& panelBounds, const PositionState& moveData, const int movesCount) const {
    std::string moveText = getMoveText(moveData, movesCount);

    // Calculate x and y positions
    Vector2 textPosition = calcMoveTextPos(panelBounds, moveText, movesCount, MoveCFG::MOVE_FONT_SIZE);

    // Draw the text
    UIRenderer::drawTextWithShadow(
        moveText,
        static_cast<int>(textPosition.x),
        static_cast<int>(textPosition.y),
        MoveCFG::MOVE_FONT_SIZE,
        Color{128, 128, 128, 255});
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

Vector2 MovesComp::calcMoveTextPos(const Rectangle& panelBounds, const std::string& moveText, const int movesCount, const int fontSize) const {
    int movePerCol = 6;
    int gridRows = 8;

    // Calculate grid cell dimensions
    int cellWidth = (MoveCFG::PANEL_WIDTH - (2 * MoveCFG::PANEL_PADDING)) / movePerCol;
    int cellHeight = (MoveCFG::PANEL_HEIGHT - (2 * MoveCFG::PANEL_PADDING) - MoveCFG::TITLE_HEIGHT) / gridRows;

    // Calculate base position of the grid cell
    int cellX = panelBounds.x + MoveCFG::PANEL_PADDING + (movesCount % movePerCol) * cellWidth;
    int cellY = panelBounds.y + MoveCFG::PANEL_PADDING + MoveCFG::TITLE_HEIGHT + (movesCount / movePerCol) * cellHeight;

    // Measure text width and center it within the cell
    int textWidth = MeasureText(moveText.c_str(), fontSize);
    int textX = cellX + (cellWidth - textWidth) / 2;  // Center horizontally in cell
    int textY = cellY; // Keep vertical position at top of cell

    return {textX, textY};
}

std::string MovesComp::getMoveText(const PositionState& moveData, const int movesCount) const {
    // If white's move, add numeric identifier. Don't add for black
    std::string moveText =
        (moveData.movedBy == 'w') ?
        "" :
        std::to_string(movesCount / 2 + 1) + ". ";
    // Get the rest of the move text
    moveText += moveData.algebraicMove;

    return moveText;
}