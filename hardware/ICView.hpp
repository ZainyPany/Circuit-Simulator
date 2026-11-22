#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "hardware/BreadboardView.hpp"
using namespace std;
using namespace sf;

// ICView: draws a chip (7408, 7404, 7447 ...) plugged into a BreadboardView.
// GUI only: it knows where its pins are, not what the chip does.
// Pin 1 is bottom-left, numbers go counter-clockwise like a real chip.
// Works for any even pin count (8, 14, 16 ...).

class ICView {
    BreadboardView& board;
    Font& font;
    int firstCol;      // column of pin 1
    string label;      // text printed on the chip
    int pins;

public:
    ICView(BreadboardView& b, Font& f, int col, string text, int pinCount = 14);

    Vector2f pinPos(int pin);      // pin numbers start at 1
    void draw(RenderTarget& target);
};
