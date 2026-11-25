#pragma once
#include <SFML/Graphics.hpp>
#include "Pin.hpp"

// Connects an output pin to an input pin
struct Wire {
    Pin* from;  // Output pin
    Pin* to;    // Input pin

    // Copies state from output pin to input pin
    void propagate();

    // Renders the wire between pins
    void draw(sf::RenderWindow& window) const;
};
