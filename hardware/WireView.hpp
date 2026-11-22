#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

// WireView: draws one wire between two points (GUI only).
// It goes down, across, then down again, with round corners.
// Give it positions from BreadboardView::holePos / railPos or ICView::pinPos.

class WireView {
    Vector2f start;
    Vector2f end;
    Color color;

public:
    WireView(Vector2f from, Vector2f to, Color wireColor);

    void draw(RenderTarget& target);
};
