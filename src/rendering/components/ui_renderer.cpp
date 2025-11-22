#include "ui_renderer.h"

// Initialize static members
Font UIRenderer::monospaceFont = {0};
bool UIRenderer::fontsLoaded = false;

void UIRenderer::initializeFonts() {
    if (fontsLoaded) return;
    
    // Try to load custom monospace font from config paths
    monospaceFont = LoadFontEx(Config::Fonts::MONOSPACE_FONT_PATH, Config::Fonts::BASE_FONT_SIZE, 0, 0);
    
    // If loading fails, try fallback fonts
    if (monospaceFont.texture.id == 0) {
        monospaceFont = LoadFontEx(Config::Fonts::FALLBACK_MONOSPACE_PATH, Config::Fonts::BASE_FONT_SIZE, 0, 0);
    }
    
    if (monospaceFont.texture.id == 0) {
        monospaceFont = LoadFontEx(Config::Fonts::FALLBACK_MONOSPACE_PATH_2, Config::Fonts::BASE_FONT_SIZE, 0, 0);
    }
    
    // If all fails, use default font
    if (monospaceFont.texture.id == 0) {
        monospaceFont = GetFontDefault();
    }
    
    fontsLoaded = true;
}

void UIRenderer::cleanupFonts() {
    if (fontsLoaded && monospaceFont.texture.id != GetFontDefault().texture.id) {
        UnloadFont(monospaceFont);
    }
    fontsLoaded = false;
}

bool UIRenderer::areCustomFontsLoaded() {
    return fontsLoaded;
}

Font UIRenderer::getMonospaceFont() {
    if (!fontsLoaded) {
        initializeFonts();
    }
    return monospaceFont;
}

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
    Font font = getMonospaceFont();
    float spacing = Config::Fonts::MONOSPACE_SPACING;
    DrawTextEx( // Draw text with subtle shadow for better readability
        font, 
        text.c_str(), 
        { static_cast<float>(x + 1), static_cast<float>(y + 1) }, 
        static_cast<float>(fontSize), 
        spacing,
        Color{0, 0, 0, 30});
    DrawTextEx(
        font, 
        text.c_str(), 
        { static_cast<float>(x), static_cast<float>(y) },
        static_cast<float>(fontSize), 
        spacing, 
        textColor);
}

int UIRenderer::measureMonospaceText(const std::string& text, int fontSize) {
    Font font = getMonospaceFont();
    float spacing = Config::Fonts::MONOSPACE_SPACING;
    Vector2 textSize = MeasureTextEx(font, text.c_str(), static_cast<float>(fontSize), spacing);
    return static_cast<int>(textSize.x);
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