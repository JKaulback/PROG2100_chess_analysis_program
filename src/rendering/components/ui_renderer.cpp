#include "ui_renderer.h"

void UIRenderer::drawPanelBackground(const Rectangle& bounds, PanelStyle style) {
    Color backgroundColor = getPanelBackgroundColor(style);
    DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, backgroundColor);
}

void UIRenderer::drawPanelBorder(const Rectangle& bounds) {
    Color borderColor = getBorderColor();
    // Draw right border to separate from main content
    DrawRectangle(bounds.x + bounds.width - 2, bounds.y, 2, bounds.height, borderColor);
}

void UIRenderer::drawPanelShadowRight(const Rectangle& bounds, int shadowLayers) {
    // Draw subtle inner shadow along the right edge
    for (int i = 0; i < shadowLayers; i++) {
        unsigned char alpha = static_cast<unsigned char>((shadowLayers * 3) - (i * 2));
        if (alpha > 0) {
            Color shadowColor = getShadowColor(alpha);
            DrawRectangle(bounds.x + bounds.width - (shadowLayers + 2) + i, bounds.y, 1, bounds.height, shadowColor);
        }
    }
}

void UIRenderer::drawPanelShadowLeft(const Rectangle& bounds, int shadowLayers) {
    // Draw subtle inner shadow along the left edge
    for (int i = 0; i < shadowLayers; i++) {
        unsigned char alpha = static_cast<unsigned char>((shadowLayers * 3) - (i * 2));
        if (alpha > 0) {
            Color shadowColor = getShadowColor(alpha);
            DrawRectangle(bounds.x + i, bounds.y, 1, bounds.height, shadowColor);
        }
    }
}

void UIRenderer::drawPanelTitle(const Rectangle& bounds, const std::string& title, int titleHeight, int padding) {
    // Draw title bar background
    Rectangle titleRect = {
        bounds.x,
        bounds.y,
        bounds.width,
        static_cast<float>(titleHeight + 4)
    };
    DrawRectangle(titleRect.x, titleRect.y, titleRect.width, titleRect.height, getTitleBarColor());
    
    // Draw bottom border of title bar
    DrawRectangle(titleRect.x, titleRect.y + titleRect.height - 1, titleRect.width, 1, getBorderColor());
    
    // Draw title text
    int titleX = bounds.x + padding;
    int titleY = bounds.y + 12;
    drawTextWithShadow(title, titleX, titleY, 22, getTitleTextColor());
}

void UIRenderer::drawTextWithShadow(const std::string& text, int x, int y, int fontSize, Color textColor) {
    // Draw text with subtle shadow for better readability
    DrawText(text.c_str(), x + 1, y + 1, fontSize, Color{0, 0, 0, 30});
    DrawText(text.c_str(), x, y, fontSize, textColor);
}

Color UIRenderer::getPanelBackgroundColor(PanelStyle style) {
    switch (style) {
        case PanelStyle::Stats:    return Color{245, 247, 250, 255}; // Light blue-gray
        case PanelStyle::Engine:   return Color{240, 242, 245, 255}; // Medium blue-gray
        case PanelStyle::Controls: return Color{250, 252, 255, 255}; // Lightest blue-gray
        case PanelStyle::Moves:    return Color{225, 237, 255, 255}; // Another light blue-gray
        default:                   return Color{245, 247, 250, 255};
    }
}

Color UIRenderer::getBorderColor() {
    return Color{200, 205, 210, 255};
}

Color UIRenderer::getShadowColor(int alpha) {
    return Color{0, 0, 0, static_cast<unsigned char>(alpha)};
}

Color UIRenderer::getTitleBarColor() {
    return Color{225, 230, 235, 255};
}

Color UIRenderer::getTitleTextColor() {
    return Color{70, 75, 80, 255};
}