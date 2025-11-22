#pragma once

#include <raylib.h>
#include <string>
#include "../../config/config.h"

class UIRenderer {
public:
    // Panel background styles
    enum class PanelStyle {
        Stats,      // Light blue-gray background
        Engine,     // Medium blue-gray background  
        Controls,    // Lightest blue-gray background
        Moves       // Another light blue-gray background
    };

    // Font management
    static void initializeFonts();
    static void cleanupFonts();
    static bool areCustomFontsLoaded();

    // Common drawing functions
    static void drawPanelBackground(const Rectangle& bounds, PanelStyle style);
    static void drawPanelBorder(const Rectangle& bounds);
    static void drawPanelShadowRight(const Rectangle& bounds, int shadowLayers = 6);
    static void drawPanelShadowLeft(const Rectangle& bounds, int shadowLayers = 6);
    static void drawPanelTitle(const Rectangle& bounds, const std::string& title, int titleHeight, int padding);
    static void drawTextWithShadow(const std::string& text, int x, int y, int fontSize, Color textColor);
    static int measureMonospaceText(const std::string& text, int fontSize);

private:
    static Font monospaceFont;
    static bool fontsLoaded;
    
    static Color getPanelBackgroundColor(PanelStyle style);
    static Color getBorderColor();
    static Color getShadowColor(int alpha);
    static Color getTitleBarColor();
    static Color getTitleTextColor();
    static Font getMonospaceFont();
};