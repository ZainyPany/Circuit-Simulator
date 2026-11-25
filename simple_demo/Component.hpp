#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Pin.hpp"

// Base class for circuit components
class Component {
public:
    std::string label;
    sf::Vector2f pos;
    sf::Vector2f size;
    std::vector<Pin> pins;

    Component(const std::string& name, sf::Vector2f position, sf::Vector2f boxSize);
    virtual ~Component();

    // Evaluates component logic state
    virtual void evaluate(bool powerOn) = 0;

    // Renders component to window
    virtual void draw(sf::RenderWindow& window, const sf::Font& font) const = 0;

    // Draws all pins on this component
    void drawPins(sf::RenderWindow& window, Pin* selectedPin) const;

    // Checks if a point is inside the component body
    bool containsBody(sf::Vector2f mousePos) const;
};
