#include "ui/hardware/WireView.hpp"
#include "ui/hardware/DrawHelpers.hpp"
#include "ui/hardware/Theme.hpp"
using namespace sf;
using namespace Theme;

WireView::WireView(Vector2f from, Vector2f to, Color color)
    : start(from), end(to), color(color) {}

void WireView::draw(RenderTarget& target) const {
    float middleY = (start.y + end.y) / 2;
    Vector2f bend1(start.x, middleY);
    Vector2f bend2(end.x, middleY);

    drawLine(target, start, bend1, WIRE_THICKNESS, color);
    drawLine(target, bend1, bend2, WIRE_THICKNESS, color);
    drawLine(target, bend2, end, WIRE_THICKNESS, color);

    // round dots at the corners and both ends
    float roundness = WIRE_THICKNESS / 2;
    drawCircle(target, start.x, start.y, roundness, color);
    drawCircle(target, bend1.x, bend1.y, roundness, color);
    drawCircle(target, bend2.x, bend2.y, roundness, color);
    drawCircle(target, end.x, end.y, roundness, color);
}
