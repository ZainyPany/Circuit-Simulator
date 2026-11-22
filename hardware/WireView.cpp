#include "hardware/WireView.hpp"
#include "hardware/Draw.hpp"
#include "hardware/Theme.hpp"

WireView::WireView(Vector2f from, Vector2f to, Color wireColor)
    : start(from), end(to), color(wireColor) {}

void WireView::draw(RenderTarget& target) {
    float middleY = (start.y + end.y) / 2;
    Vector2f bend1(start.x, middleY);
    Vector2f bend2(end.x, middleY);

    drawLine(target, start, bend1, WIRE_THICKNESS, color);
    drawLine(target, bend1, bend2, WIRE_THICKNESS, color);
    drawLine(target, bend2, end, WIRE_THICKNESS, color);
    float radius = WIRE_THICKNESS / 2;
    drawCircle(target, start.x, start.y, radius, color);
    drawCircle(target, bend1.x, bend1.y, radius, color);
    drawCircle(target, bend2.x, bend2.y, radius, color);
    drawCircle(target, end.x, end.y, radius, color);
}
