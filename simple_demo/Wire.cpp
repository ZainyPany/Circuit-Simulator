#include "Wire.hpp"
#include "DrawHelpers.hpp"

// Copies state from output pin to input pin
void Wire::propagate() {
    if (from->role == PinRole::Gnd) {
        to->state = false;
        to->connectedToGnd = from->connectedToGnd;
    } else {
        to->state = from->state;
    }
}

// Renders wire with color based on signal state
void Wire::draw(sf::RenderWindow& window) const {
    sf::Color color(135, 145, 165); // Default gray (0 / LOW)

    if (from->role == PinRole::Vcc) {
        color = from->state ? sf::Color(235, 70, 70) : sf::Color(120, 55, 55); // Bright red (5V) / dark red (0V)
    } else if (from->role == PinRole::Gnd) {
        color = sf::Color(70, 85, 110); // Blue-gray (GND)
    } else if (from->state) {
        color = sf::Color(70, 235, 110); // Bright green (1 / HIGH)
    }

    drawThickLine(window, from->pos, to->pos, 3.5f, color);
}
