#pragma once
#include <SFML/Graphics.hpp>

// BreadboardView: draws a solderless breadboard (GUI only, no circuit logic).
//
// Holes are addressed by column (0..29) and row. Use the row names from Theme.hpp
// (ROW_A .. ROW_J) and the rail names (TOP_PLUS, TOP_MINUS, BOTTOM_PLUS, BOTTOM_MINUS).

class BreadboardView {
    sf::Vector2f origin;   // top-left corner of the board on the screen

    void drawHole(sf::RenderTarget& target, sf::Vector2f position) const;
    void drawRailLine(sf::RenderTarget& target, float railY, float distance, sf::Color color) const;

public:
    BreadboardView(sf::Vector2f position);

    // Screen position of the centre of a hole in the main area.
    sf::Vector2f holePos(int col, int row) const;

    // Screen position of the centre of a hole in a power rail.
    sf::Vector2f railPos(int col, int rail) const;

    void draw(sf::RenderTarget& target) const;
};
