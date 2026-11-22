#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class WireView {
    Vector2f start;
    Vector2f end;
    Color color;

public:
    WireView(Vector2f from, Vector2f to, Color wireColor);

    void draw(RenderTarget& target);
};
