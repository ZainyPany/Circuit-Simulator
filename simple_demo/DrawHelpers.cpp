#include "DrawHelpers.hpp"
#include <cmath>

float distanceBetween(sf::Vector2f a, sf::Vector2f b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

void drawCenteredText(sf::RenderWindow& window, const sf::Font& font,
                      const std::string& str, unsigned int size,
                      sf::Color color, float x, float y) {
    sf::Text text(font, str, size);
    text.setFillColor(color);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2.0f,
                    bounds.position.y + bounds.size.y / 2.0f});
    text.setPosition({x, y});
    window.draw(text);
}

void drawThickLine(sf::RenderWindow& window, sf::Vector2f a, sf::Vector2f b,
                   float thickness, sf::Color color) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float len = std::sqrt(dx * dx + dy * dy);
    if (len < 0.001f) return;

    float offsetX = (-dy / len) * (thickness / 2.0f);
    float offsetY = ( dx / len) * (thickness / 2.0f);

    sf::Vertex quad[4] = {
        {{a.x + offsetX, a.y + offsetY}, color},
        {{a.x - offsetX, a.y - offsetY}, color},
        {{b.x + offsetX, b.y + offsetY}, color},
        {{b.x - offsetX, b.y - offsetY}, color}
    };
    window.draw(quad, 4, sf::PrimitiveType::TriangleStrip);
}
