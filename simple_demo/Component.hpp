#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Pin.hpp"

// Abstract OOP base class for every component on the canvas
class Component {
public:
    std::string label;
    sf::Vector2f pos;
    sf::Vector2f size;
    std::vector<Pin> pins;

    Component(const std::string& name, sf::Vector2f position, sf::Vector2f boxSize);
    virtual ~Component();

    virtual void evaluate(bool powerOn) = 0;
    virtual void draw(sf::RenderWindow& window, const sf::Font& font) const = 0;

    void drawPins(sf::RenderWindow& window, Pin* selectedPin) const;
    bool containsBody(sf::Vector2f mousePos) const;
};
