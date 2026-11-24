#pragma once
#include <SFML/Graphics.hpp>
#include "Pin.hpp"

// Connects one Output Pin to one Input Pin
struct Wire {
    Pin* from;  // Output pin
    Pin* to;    // Input pin

    void propagate();
    void draw(sf::RenderWindow& window) const;
};
