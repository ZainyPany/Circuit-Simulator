#include "Component.hpp"

Component::Component(const std::string& name, sf::Vector2f position, sf::Vector2f boxSize)
    : label(name), pos(position), size(boxSize) {}

Component::~Component() {}

void Component::drawPins(sf::RenderWindow& window, Pin* selectedPin) const {
    for (std::size_t i = 0; i < pins.size(); ++i) {
        const Pin& p = pins[i];

        // Yellow highlight ring while this leg is anchored for a wire
        if (&pins[i] == selectedPin) {
            sf::CircleShape ring(10.0f);
            ring.setOrigin({10.0f, 10.0f});
            ring.setPosition(p.pos);
            ring.setFillColor(sf::Color(255, 220, 70));
            window.draw(ring);
        }

        // Fill color based on role & live logic level
        sf::Color fill(155, 165, 185);
        if (p.role == PinRole::Vcc) {
            fill = p.state ? sf::Color(240, 75, 75) : sf::Color(135, 60, 60);
        } else if (p.role == PinRole::Gnd) {
            fill = p.connectedToGnd ? sf::Color(85, 130, 210) : sf::Color(70, 76, 90);
        } else if (p.state) {
            fill = sf::Color(70, 235, 110);
        }

        sf::CircleShape dot(6.0f);
        dot.setOrigin({6.0f, 6.0f});
        dot.setPosition(p.pos);
        dot.setFillColor(fill);
        dot.setOutlineThickness(1.8f);
        // Subtle gold outline on output legs so they're easy to spot
        dot.setOutlineColor(p.isOutput ? sf::Color(245, 200, 80) : sf::Color(28, 32, 42));
        window.draw(dot);
    }
}

bool Component::containsBody(sf::Vector2f mousePos) const {
    sf::FloatRect box(pos, size);
    return box.contains(mousePos);
}
