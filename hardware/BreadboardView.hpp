#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

// BreadboardView: draws a breadboard (GUI only, no logic).
// A hole is a column (0..29) and a row (ROW_A .. ROW_J from Theme.hpp).

class BreadboardView {
    Vector2f origin;   // top-left corner on the screen

public:
    BreadboardView(Vector2f position);

    Vector2f holePos(int col, int row);     // centre of a hole in the main area
    Vector2f railPos(int col, int rail);    // centre of a hole in a power rail
    void draw(RenderTarget& target);
};
