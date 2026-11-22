#pragma once
#include <string>
#include <SFML/Graphics.hpp>

// Small drawing helpers shared by the hardware GUI classes.

void drawRect(sf::RenderTarget& target, float x, float y, float w, float h, sf::Color color);

void drawCircle(sf::RenderTarget& target, float x, float y, float radius, sf::Color color);

// Draws text whose middle is at (x, y).
void drawText(sf::RenderTarget& target, sf::Font& font, const std::string& str,
              int size, sf::Color color, float x, float y);

// Draws a thick line. (SFML's own lines are always 1 pixel wide.)
void drawLine(sf::RenderTarget& target, sf::Vector2f a, sf::Vector2f b,
              float thickness, sf::Color color);
