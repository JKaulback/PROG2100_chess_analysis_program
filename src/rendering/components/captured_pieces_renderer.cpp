#include "captured_pieces_renderer.h"
#include "texture_manager.h"  
#include "../../application/chess_analysis_program.h"
#include "../../config/config.h"

namespace BoardCfg = Config::Board;
namespace PieceCfg = Config::Pieces;

CapturedPiecesRenderer::CapturedPiecesRenderer(const ChessAnalysisProgram& controller, const TextureManager& textureManager) :
    controller(controller), textureManager(textureManager) {
}

void CapturedPiecesRenderer::draw() const {
    drawCapturedPieces();
}

void CapturedPiecesRenderer::drawCapturedPieces() const {
    // Calculate static dimensions for 8 pieces high, 2 columns wide
    const float staticWidth = (2 * PieceCfg::CAPTURED_SIZE) + (PieceCfg::CAPTURED_SIZE * 0.1f) + (PieceCfg::CAPTURED_SIZE * 0.5f);
    const float staticHeight = (8 * PieceCfg::CAPTURED_STEP) + (PieceCfg::CAPTURED_SIZE * 0.2f);
    
    // Determine positions based on board flip state
    bool isFlipped = controller.getBoardFlipped();
    
    // Calculate positions for white and black areas
    float whiteAreaX = PieceCfg::CAPTURED_OFFSET_X;
    float blackAreaX = PieceCfg::CAPTURED_OFFSET_X;
    
    float whiteAreaY, blackAreaY;
    if (isFlipped) {
        // When flipped, white goes to bottom, black to top
        whiteAreaY = PieceCfg::CAPTURED_OFFSET_Y_BLACK - staticHeight + PieceCfg::CAPTURED_SIZE * 1.3f;
        blackAreaY = PieceCfg::CAPTURED_OFFSET_Y_WHITE - PieceCfg::CAPTURED_SIZE * 0.3f;
    } else {
        // Normal: white at top, black at bottom
        whiteAreaY = PieceCfg::CAPTURED_OFFSET_Y_WHITE - PieceCfg::CAPTURED_SIZE * 0.3f;
        blackAreaY = PieceCfg::CAPTURED_OFFSET_Y_BLACK - staticHeight + PieceCfg::CAPTURED_SIZE * 1.3f;
    }
    
    // Always draw both backgrounds (static)
    drawCapturedPiecesBackground(whiteAreaX, whiteAreaY, staticWidth, staticHeight, true);
    drawCapturedPiecesBackground(blackAreaX, blackAreaY, staticWidth, staticHeight, false);
    
    // Draw labels
    drawCapturedPiecesLabels(whiteAreaX, whiteAreaY, blackAreaX, blackAreaY, staticWidth, isFlipped);
    
    // Get captured pieces by color directly from the board
    std::vector<char> whiteCaptured = controller.getWhiteCapturedPieces();
    std::vector<char> blackCaptured = controller.getBlackCapturedPieces();
    
    // If no captured pieces, still draw the empty backgrounds
    if (whiteCaptured.empty() && blackCaptured.empty()) {
        return;
    }

    // Draw white captured pieces
    for (int i = 0; i < whiteCaptured.size(); i++) {
        char piece = whiteCaptured[i];
        std::string pieceString = controller.pieceToTextureString(piece);
        
        // Calculate column and row within the box
        int col = (i <= PieceCfg::MAX_CAPTURED_IN_ROW) ? 0 : 1;
        int row = (i <= PieceCfg::MAX_CAPTURED_IN_ROW) ? i : i - PieceCfg::MAX_CAPTURED_IN_ROW - 1;
        
        // Position pieces relative to the white background box
        float xPos = whiteAreaX + (PieceCfg::CAPTURED_SIZE * 0.25f) + (col * PieceCfg::CAPTURED_STEP);
        float yPos = whiteAreaY + (PieceCfg::CAPTURED_SIZE * 0.05f) + (row * PieceCfg::CAPTURED_STEP);
        
        DrawTextureEx(textureManager.getPieceTexture(pieceString), {xPos, yPos}, 0.0f, 
            PieceCfg::CAPTURED_SCALE, WHITE);
    }
    
    // Draw black captured pieces
    for (int i = 0; i < blackCaptured.size(); i++) {
        char piece = blackCaptured[i];
        std::string pieceString = controller.pieceToTextureString(piece);
        
        // Calculate column and row within the box
        int col = (i <= PieceCfg::MAX_CAPTURED_IN_ROW) ? 0 : 1;
        int row = (i <= PieceCfg::MAX_CAPTURED_IN_ROW) ? i : i - PieceCfg::MAX_CAPTURED_IN_ROW - 1;
        
        // Position pieces relative to the black background box
        float xPos = blackAreaX + (PieceCfg::CAPTURED_SIZE * 0.25f) + (col * PieceCfg::CAPTURED_STEP);
        float yPos = blackAreaY + (PieceCfg::CAPTURED_SIZE * 0.05f) + (row * PieceCfg::CAPTURED_STEP);
        
        DrawTextureEx(textureManager.getPieceTexture(pieceString), {xPos, yPos}, 0.0f, 
            PieceCfg::CAPTURED_SCALE, WHITE);
    }
}

void CapturedPiecesRenderer::drawCapturedPiecesBackground(float x, float y, float width, float height, bool isWhite) const {
    const float borderWidth = 6.0f;
    
    // Calculate border rectangle  
    const float borderX = x - borderWidth;
    const float borderY = y - borderWidth;
    const float borderWidthTotal = width + (borderWidth * 2);
    const float borderHeightTotal = height + (borderWidth * 2);
    
    // Draw border layers
    drawBorderLayers(borderX, borderY, borderWidthTotal, borderHeightTotal);
    drawBorderHighlights(borderX, borderY, borderWidthTotal, borderHeightTotal);
    drawCornerDecorations(borderX, borderY, borderWidthTotal, borderHeightTotal);
    
    // Draw background for pieces (slightly different color based on side)
    Color backgroundColor = isWhite ? 
        Color{240, 228, 210, 255} :  // Light cream for white pieces
        Color{139, 114, 85, 255};    // Darker brown for black pieces
    
    DrawRectangle(
        static_cast<int>(x), 
        static_cast<int>(y), 
        static_cast<int>(width), 
        static_cast<int>(height), 
        backgroundColor
    );
}

void CapturedPiecesRenderer::drawBorderLayers(float borderX, float borderY, float borderWidth, float borderHeight) const {
    const float shadowOffset = 4.0f;
    const Color borderColor = {139, 69, 19, 255};      // Saddle brown - elegant wood color
    const Color innerBorder = {160, 82, 45, 255};      // Lighter brown accent
    const Color shadowColor = {0, 0, 0, 60};           // Subtle shadow
    const float accentWidth = 1.5f;
    
    // Draw shadow (offset behind the border)
    DrawRectangle(
        static_cast<int>(borderX + shadowOffset), 
        static_cast<int>(borderY + shadowOffset), 
        static_cast<int>(borderWidth), 
        static_cast<int>(borderHeight), 
        shadowColor
    );
    
    // Draw main border (dark brown)
    DrawRectangle(
        static_cast<int>(borderX), 
        static_cast<int>(borderY), 
        static_cast<int>(borderWidth), 
        static_cast<int>(borderHeight), 
        borderColor
    );
    
    // Draw inner accent border (lighter brown)
    DrawRectangle(
        static_cast<int>(borderX + accentWidth), 
        static_cast<int>(borderY + accentWidth), 
        static_cast<int>(borderWidth - accentWidth * 2), 
        static_cast<int>(borderHeight - accentWidth * 2), 
        innerBorder
    );
}

void CapturedPiecesRenderer::drawBorderHighlights(float borderX, float borderY, float borderWidth, float borderHeight) const {
    const Color highlightColor = {205, 133, 63, 255};  // Peru - lighter highlight
    const float accentWidth = 1.5f;
    const float highlightWidth = 1.0f;
    
    // Top highlight
    DrawRectangle(
        static_cast<int>(borderX + accentWidth), 
        static_cast<int>(borderY + accentWidth), 
        static_cast<int>(borderWidth - accentWidth * 2), 
        static_cast<int>(highlightWidth), 
        highlightColor
    );
    
    // Left highlight
    DrawRectangle(
        static_cast<int>(borderX + accentWidth), 
        static_cast<int>(borderY + accentWidth), 
        static_cast<int>(highlightWidth), 
        static_cast<int>(borderHeight - accentWidth * 2), 
        highlightColor
    );
}

void CapturedPiecesRenderer::drawCornerDecorations(float borderX, float borderY, float borderWidth, float borderHeight) const {
    const float cornerSize = 12.0f;
    const float cornerOffset = 3.0f;
    const float cornerInset = 9.0f;
    const Color decorColor = {205, 133, 63, 180};  // Semi-transparent peru
    const Color accentColor = {139, 69, 19, 255};  // Saddle brown
    
    // Draw all four corners using the helper method
    drawSingleCornerDecoration(borderX - cornerOffset, borderY - cornerOffset, cornerSize, decorColor, accentColor);  // Top-left
    drawSingleCornerDecoration(borderX + borderWidth - cornerInset, borderY - cornerOffset, cornerSize, decorColor, accentColor);  // Top-right
    drawSingleCornerDecoration(borderX - cornerOffset, borderY + borderHeight - cornerInset, cornerSize, decorColor, accentColor);  // Bottom-left
    drawSingleCornerDecoration(borderX + borderWidth - cornerInset, borderY + borderHeight - cornerInset, cornerSize, decorColor, accentColor);  // Bottom-right
}

void CapturedPiecesRenderer::drawSingleCornerDecoration(float x, float y, float cornerSize, const Color& decorColor, const Color& accentColor) const {
    DrawRectangle(
        static_cast<int>(x), 
        static_cast<int>(y), 
        static_cast<int>(cornerSize), 
        static_cast<int>(cornerSize), 
        decorColor
    );
    DrawRectangleLines(
        static_cast<int>(x), 
        static_cast<int>(y), 
        static_cast<int>(cornerSize), 
        static_cast<int>(cornerSize), 
        accentColor
    );
}

std::pair<int, int> CapturedPiecesRenderer::getCapturedPiecePosition(const int numPieces, const char pieceOwner) const {
    // When drawing more than the config max number, move to next col
    int col = 
        (numPieces <= PieceCfg::MAX_CAPTURED_IN_ROW) ? 
        0 : 
        1;
    // Determine the y grid location (row within the background box)
    int row = 
        (numPieces <= PieceCfg::MAX_CAPTURED_IN_ROW) ?
        numPieces : 
        numPieces - PieceCfg::MAX_CAPTURED_IN_ROW - 1;
    
    // Return relative position within the background box
    return {col, row};
}

void CapturedPiecesRenderer::drawCapturedPiecesLabels(float whiteX, float whiteY, float blackX, float blackY, float width, bool isFlipped) const {
    const int labelFontSize = 16;
    const Color labelColor = {139, 69, 19, 255}; // Same brown as border
    const float labelOffset = 15.0f; // Increased from 5.0f to 15.0f for more space
    
    // Determine which labels go where based on flip state
    const char* topLabel = isFlipped ? "Black Captured" : "White Captured";
    const char* bottomLabel = isFlipped ? "White Captured" : "Black Captured";
    
    float topLabelX = isFlipped ? blackX : whiteX;
    float topLabelY = (isFlipped ? blackY : whiteY) - labelFontSize - labelOffset;
    
    float bottomLabelX = isFlipped ? whiteX : blackX;
    float bottomLabelY = (isFlipped ? whiteY : blackY) - labelFontSize - labelOffset;
    
    // Center the labels above their respective areas
    int topLabelWidth = MeasureText(topLabel, labelFontSize);
    int bottomLabelWidth = MeasureText(bottomLabel, labelFontSize);
    
    topLabelX += (width - topLabelWidth) / 2.0f;
    bottomLabelX += (width - bottomLabelWidth) / 2.0f;
    
    // Draw the labels
    DrawText(topLabel, static_cast<int>(topLabelX), static_cast<int>(topLabelY), labelFontSize, labelColor);
    DrawText(bottomLabel, static_cast<int>(bottomLabelX), static_cast<int>(bottomLabelY), labelFontSize, labelColor);
}

std::pair<float, float> CapturedPiecesRenderer::getCapturedAreaDimensions(const std::vector<char>& capturedPieces, bool isWhite) const {
    if (capturedPieces.empty()) {
        return {0.0f, 0.0f};
    }
    
    // Calculate number of columns needed
    int numCols = (capturedPieces.size() > PieceCfg::MAX_CAPTURED_IN_ROW) ? 2 : 1;
    
    // Calculate width (columns * piece size + spacing)
    float width = (numCols * PieceCfg::CAPTURED_SIZE) + ((numCols - 1) * PieceCfg::CAPTURED_SIZE * 0.1f) + (PieceCfg::CAPTURED_SIZE * 0.5f);
    
    // Calculate height based on maximum pieces in any column
    int maxPiecesInCol = std::min(static_cast<int>(capturedPieces.size()), PieceCfg::MAX_CAPTURED_IN_ROW + 1);
    float height = (maxPiecesInCol * PieceCfg::CAPTURED_STEP) + (PieceCfg::CAPTURED_SIZE * 0.1f);
    
    return {width, height};
}