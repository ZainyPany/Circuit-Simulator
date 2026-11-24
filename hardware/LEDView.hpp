#pragma once
#include <SFML/Graphics.hpp>
#include "BreadboardView.hpp"
using namespace sf;

enum class LEDColor { Red, Green, Yellow, Blue };

class LEDView {
    BreadboardView& board;
    int col;
    int row;
    LEDColor color;
    bool lit;

public:
    LEDView(BreadboardView& b, int column, int rowIndex, LEDColor ledColor, bool isOn = false);

    Vector2f anodePos();
    Vector2f cathodePos();
    void setOn(bool isOn);
    void draw(RenderTarget& target);
};
