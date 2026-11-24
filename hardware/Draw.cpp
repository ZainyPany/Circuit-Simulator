#include "Draw.hpp"
#include <algorithm>
#include <cmath>

using namespace sf;
using namespace std;

// Draw a filled rectangle using SFML's RectangleShape
void drawRect(RenderTarget& target, float x, float y, float w, float h, Color color) {
    RectangleShape rect({w, h});
    rect.setPosition({x, y});
    rect.setFillColor(color);
    target.draw(rect);
}

// Draw a filled circle centered at (x, y) using SFML's CircleShape
void drawCircle(RenderTarget& target, float x, float y, float radius, Color color) {
    CircleShape circle(radius);
    circle.setOrigin({radius, radius}); // Anchor to center so (x, y) is the circle's center
    circle.setPosition({x, y});
    circle.setFillColor(color);
    target.draw(circle);
}

// Draw text centered at (x, y)
void drawText(RenderTarget& target, Font& font, string str, int size, Color color, float x, float y) {
    Text text(font, str, size);
    text.setFillColor(color);
    FloatRect box = text.getLocalBounds();
    text.setOrigin({box.position.x + box.size.x / 2.0f, box.position.y + box.size.y / 2.0f});
    text.setPosition({x, y});
    target.draw(text);
}

// Draw a straight line using a simple rectangle
void drawLine(RenderTarget& target, Vector2f a, Vector2f b, float thickness, Color color) {
    float x = min(a.x, b.x);
    float y = min(a.y, b.y);
    float w = abs(b.x - a.x);
    float h = abs(b.y - a.y);

    if (w == 0) {
        // Vertical line segment: draw a rectangle with width = thickness
        drawRect(target, a.x - thickness / 2.0f, y, thickness, h, color);
    } else if (h == 0) {
        // Horizontal line segment: draw a rectangle with height = thickness
        drawRect(target, x, a.y - thickness / 2.0f, w, thickness, color);
    }
}
