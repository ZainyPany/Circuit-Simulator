#include "WireView.hpp"
#include "Draw.hpp"
#include "Theme.hpp"
#include <algorithm>
#include <cmath>

WireView::WireView(Vector2f from, Vector2f to, Color wireColor)
    : start(from), end(to), color(wireColor) {}

void WireView::draw(RenderTarget& target) {
    // A wire runs vertically, turns horizontally at the midpoint, then runs vertically to the destination.
    float middleY = (start.y + end.y) / 2.0f;

    // 1. First vertical rectangle (from start to middle bend)
    float y1 = std::min(start.y, middleY);
    float h1 = std::abs(middleY - start.y);
    drawRect(target, start.x - WIRE_THICKNESS / 2.0f, y1, WIRE_THICKNESS, h1, color);

    // 2. Middle horizontal rectangle (connecting start column to end column)
    float x2 = std::min(start.x, end.x);
    float w2 = std::abs(end.x - start.x);
    drawRect(target, x2, middleY - WIRE_THICKNESS / 2.0f, w2, WIRE_THICKNESS, color);

    // 3. Second vertical rectangle (from middle bend to end)
    float y3 = std::min(middleY, end.y);
    float h3 = std::abs(end.y - middleY);
    drawRect(target, end.x - WIRE_THICKNESS / 2.0f, y3, WIRE_THICKNESS, h3, color);

    // 4. Four circles for smooth rounded joints at the start, bends, and end
    float radius = WIRE_THICKNESS / 2.0f;
    drawCircle(target, start.x, start.y, radius, color);
    drawCircle(target, start.x, middleY, radius, color);
    drawCircle(target, end.x, middleY, radius, color);
    drawCircle(target, end.x, end.y, radius, color);
}
