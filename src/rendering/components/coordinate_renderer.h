#pragma once

#include <raylib.h>

/**
 * Handles rendering coordinate labels (files a-h and ranks 1-8) around the board
 */
class CoordinateRenderer {
public:
    CoordinateRenderer() = default;
    
    void draw() const;

private:
    void drawCoordinates() const;
};