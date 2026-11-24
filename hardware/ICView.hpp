#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "BreadboardView.hpp"
using namespace std;
using namespace sf;

class ICView {
    BreadboardView& board;
    Font& font;
    int firstCol;
    string label;
    int pins;

public:
    ICView(BreadboardView& b, Font& f, int col, string text, int pinCount = 14);

    Vector2f pinPos(int pin);
    void draw(RenderTarget& target);
};
