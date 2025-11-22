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

    int movesDrawnCount = 0;
    int startIndex = 1;

    // There is only enough space to display 54 moves.
    // Calculate which moves to be displayed ("..." for overflow)
    bool isEllipsisBefore = false;
    bool isEllipsisAfter = false;

    // positionHistory includes initial position (no moves)
    int totalMoves = positionHistory.size() - 1 + positionRedos.size();

    // You're looking at 8 hours of debugging and fighting with Copilot
    if (totalMoves > MoveCFG::MAX_MOVES_DISPLAYED) {
        // If no redo moves, just show the most recent moves with ellipsis before
        if (positionRedos.empty()) {
            isEllipsisBefore = true;
            // Calculate how many moves to skip, reserving one spot for ellipsis
            int movesToSkip = totalMoves - MoveCFG::MAX_MOVES_DISPLAYED + 1;
            startIndex = 1 + movesToSkip;
            // Start on black's move (even indices are black moves)
            if (positionHistory.at(startIndex).movedBy == 'b')
                startIndex += 1;
        // If there are redo moves, use pivot logic
        } else if (positionHistory.size() - 1 < MoveCFG::PIVOT_MOVE_INDEX + 2) {
            // History fits in pivot, redo will overflow
            isEllipsisAfter = true;
        } else if (positionRedos.size() <= MoveCFG::MAX_MOVES_DISPLAYED - MoveCFG::PIVOT_MOVE_INDEX) {
            // Redo fits after pivot, history will overflow
            isEllipsisBefore = true;
            startIndex = (positionHistory.size() - 1) - (MoveCFG::MAX_MOVES_DISPLAYED - positionRedos.size() - 1) + 1;
            if (positionHistory.at(startIndex).movedBy == 'b')
                startIndex += 1;
        } else {
            // Both overflow, center around current position
            isEllipsisBefore = true;
            isEllipsisAfter = true;
            startIndex = positionHistory.size() - MoveCFG::PIVOT_MOVE_INDEX;
            if (positionHistory.at(startIndex).movedBy == 'b')
                startIndex += 1;
        }
    }
    
    if (isEllipsisBefore) {
        drawEllipsis(panelBounds, movesDrawnCount++);
    }

    // Draw historical moves
    for (int i = startIndex; i < positionHistory.size() && movesDrawnCount < MoveCFG::MAX_MOVES_DISPLAYED; i++) {
        // Reserve space for ellipsis at the end if needed
        if (isEllipsisAfter && movesDrawnCount >= MoveCFG::MAX_MOVES_DISPLAYED - 1) {
            break;
        }
        drawHistoricalMove(
            panelBounds, 
            positionHistory.at(i), 
            movesDrawnCount++, i >= positionHistory.size() - 1, i);
    }
    
    // Draw redo moves
    if (!positionRedos.empty() && movesDrawnCount < MoveCFG::MAX_MOVES_DISPLAYED) {
        for (int i = positionRedos.size() - 1; i >= 0 && movesDrawnCount < MoveCFG::MAX_MOVES_DISPLAYED; i--) {
            // Reserve space for ellipsis at the end if needed
            if (isEllipsisAfter && movesDrawnCount >= MoveCFG::MAX_MOVES_DISPLAYED - 1)
                break;
            else if (movesDrawnCount >= MoveCFG::MAX_MOVES_DISPLAYED)
                break;
            int index = positionHistory.size() + positionRedos.size() - i - 1;
            drawRedoMove(panelBounds, positionRedos.at(i), movesDrawnCount++, index);
        }
    }
    
    // Draw trailing ellipsis if needed
    if (isEllipsisAfter && movesDrawnCount < MoveCFG::MAX_MOVES_DISPLAYED) {
        drawEllipsis(panelBounds, movesDrawnCount++);
    }
}

void MovesComp::drawHistoricalMove(
    const Rectangle& panelBounds, 
    const PositionState& moveData, 
    const int movesCount, 
    const bool isLastMove, 
    const int index) const {

    std::string moveText = getMoveText(moveData, movesCount, index);

    // Calculate x and y positions
    Vector2 textPosition = calcMoveTextPos(panelBounds, moveText, movesCount, MoveCFG::MOVE_FONT_SIZE);

    // Determine color
    Color moveColor =
        isLastMove ?
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

void MovesComp::drawRedoMove(
    const Rectangle& panelBounds, 
    const PositionState& moveData, 
    const int movesCount,
    const int index) const {

    std::string moveText = getMoveText(moveData, movesCount, index);

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

void MovesComp::drawEllipsis(const Rectangle& panelBounds, const int movesCount) const {
    std::string moveText = MoveCFG::ELLIPSIS;

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
    int movePerCol = MoveCFG::MOVES_PER_ROW;
    int gridRows = MoveCFG::GRID_ROWS;

    // Calculate grid cell dimensions
    int cellWidth = (MoveCFG::PANEL_WIDTH - (2 * MoveCFG::PANEL_PADDING)) / movePerCol;
    int cellHeight = (MoveCFG::PANEL_HEIGHT - (2 * MoveCFG::PANEL_PADDING) - MoveCFG::TITLE_HEIGHT) / gridRows;

    // Calculate base position of the grid cell
    int cellX = panelBounds.x + MoveCFG::PANEL_PADDING + (movesCount % movePerCol) * cellWidth;
    int cellY = panelBounds.y + MoveCFG::PANEL_PADDING + MoveCFG::TITLE_HEIGHT + (movesCount / movePerCol) * cellHeight;

    // Add bounds checking to prevent overflow
    if (movesCount >= MoveCFG::MAX_MOVES_DISPLAYED) {
        // If we're beyond our grid capacity, don't draw (return off-screen position)
        return {-1000.0f, -1000.0f};
    }

    // Measure text width and center it within the cell
    int textWidth = MeasureText(moveText.c_str(), fontSize);
    int textX = cellX + (cellWidth - textWidth) / 2;  // Center horizontally in cell
    int textY = cellY; // Keep vertical position at top of cell

    return {static_cast<float>(textX), static_cast<float>(textY)};
}

std::string MovesComp::getMoveText(const PositionState& moveData, const int movesCount, const int index) const {
    // If white's move, add numeric identifier. Don't add for black
    std::string moveText =
        (moveData.movedBy == 'w') ?
        "" :
        std::to_string(index / 2 + 1) + ". " ;
    // Get the rest of the move text
    moveText += moveData.algebraicMove;

    return moveText;
}