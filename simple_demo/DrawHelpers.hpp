#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Distance between two 2D points on screen
float distanceBetween(sf::Vector2f a, sf::Vector2f b);

// Draw centered text at (x, y)
void drawCenteredText(sf::RenderWindow& window, const sf::Font& font,
                      const std::string& str, unsigned int size,
                      sf::Color color, float x, float y);

// Draw a thick line segment between points a and b
void drawThickLine(sf::RenderWindow& window, sf::Vector2f a, sf::Vector2f b,
                   float thickness, sf::Color color);
