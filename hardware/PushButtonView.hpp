#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "hardware/BreadboardView.hpp"
using namespace std;
using namespace sf;

class PushButtonView {
    BreadboardView& board;
    Font& font;
    int col;
    int row;
    string label;
    bool pressed;

    Vector2f centerPos() const;

public:
    PushButtonView(BreadboardView& b, Font& f, int column, int rowIndex, const string& text, bool isPressed = false);

    void setPressed(bool isPressed);
    bool isPressed() const;

    Vector2f leftPinPos() const;
    Vector2f rightPinPos() const;
    bool contains(Vector2f point) const;

    void draw(RenderTarget& target);
};
