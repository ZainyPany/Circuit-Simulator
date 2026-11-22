#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class BreadboardView {
    Vector2f origin;

public:
    BreadboardView(Vector2f position);

    Vector2f holePos(int col, int row);
    Vector2f railPos(int col, int rail);
    void draw(RenderTarget& target);
};
