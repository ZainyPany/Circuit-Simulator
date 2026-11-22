#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "ui/hardware/BreadboardView.hpp"

// ICView: draws a DIP chip (7408, 7404, 7447 ...) plugged into a BreadboardView.
// GUI only: it knows where its pins are, not what the chip does.
//
// The chip straddles the middle gap. Pin 1 is bottom-left and the numbers go
// counter-clockwise, like a real chip. Works for any even pin count (8, 14, 16 ...).

class ICView {
    const BreadboardView& board;
    sf::Font& font;
    int firstCol;          // column of pin 1
    std::string label;     // text printed on the chip, e.g. "7408 AND"
    int pins;

public:
    ICView(const BreadboardView& board, sf::Font& font, int firstCol,
           const std::string& label, int pins = 14);

    // Screen position of a pin (pin numbers start at 1).
    sf::Vector2f pinPos(int pin) const;

    void draw(sf::RenderTarget& target) const;
};
