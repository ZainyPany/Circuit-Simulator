#pragma once
#include <SFML/Graphics.hpp>

// WireView: draws one jumper wire between two points (GUI only).
// The wire goes down, across, then down again, with rounded corners.
// Pass it hole positions from BreadboardView::holePos / railPos / ICView::pinPos.

class WireView {
    sf::Vector2f start;
    sf::Vector2f end;
    sf::Color color;

public:
    WireView(sf::Vector2f from, sf::Vector2f to, sf::Color color);

    void draw(sf::RenderTarget& target) const;
};
