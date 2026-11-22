#include "ui/hardware/DrawHelpers.hpp"
#include <cmath>
using namespace std;
using namespace sf;

void drawRect(RenderTarget& target, float x, float y, float w, float h, Color color) {
    RectangleShape rect({w, h});
    rect.setPosition({x, y});
    rect.setFillColor(color);
    target.draw(rect);
}

void drawCircle(RenderTarget& target, float x, float y, float radius, Color color) {
    CircleShape circle(radius);
    circle.setOrigin({radius, radius});
    circle.setPosition({x, y});
    circle.setFillColor(color);
    target.draw(circle);
}

void drawText(RenderTarget& target, Font& font, const string& str,
              int size, Color color, float x, float y) {
    Text text(font, str, size);
    text.setFillColor(color);
    FloatRect box = text.getLocalBounds();
    text.setOrigin({box.position.x + box.size.x / 2, box.position.y + box.size.y / 2});
    text.setPosition({x, y});
    target.draw(text);
}

void drawLine(RenderTarget& target, Vector2f a, Vector2f b, float thickness, Color color) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float length = sqrt(dx * dx + dy * dy);
    if (length == 0) return;

    float half = thickness / 2;
    float sideX = -dy / length * half;   // push the corners sideways
    float sideY = dx / length * half;

    Vertex corners[4] = {
        {{a.x + sideX, a.y + sideY}, color},
        {{a.x - sideX, a.y - sideY}, color},
        {{b.x + sideX, b.y + sideY}, color},
        {{b.x - sideX, b.y - sideY}, color}};
    target.draw(corners, 4, PrimitiveType::TriangleStrip);
}
