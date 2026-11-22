#include "hardware/LEDView.hpp"
#include "hardware/Draw.hpp"
#include "hardware/Theme.hpp"
#include <algorithm>

LEDView::LEDView(BreadboardView& b, int column, int rowIndex, LEDColor ledColor, bool isOn)
    : board(b), col(column), row(rowIndex), color(ledColor), lit(isOn) {}

Vector2f LEDView::anodePos() {
    return board.holePos(col, row);
}

Vector2f LEDView::cathodePos() {
    return board.holePos(col + 1, row);
}

void LEDView::setOn(bool isOn) {
    lit = isOn;
}

void LEDView::draw(RenderTarget& target) {
    Color on = LED_RED_ON;
    Color off = LED_RED_OFF;
    if (color == LEDColor::Green) { on = LED_GREEN_ON; off = LED_GREEN_OFF; }
    if (color == LEDColor::Yellow) { on = LED_YELLOW_ON; off = LED_YELLOW_OFF; }
    if (color == LEDColor::Blue) { on = LED_BLUE_ON; off = LED_BLUE_OFF; }

    Vector2f a = anodePos();
    Vector2f c = cathodePos();
    float dir = (row <= ROW_E) ? -1.0f : 1.0f;
    float centerX = (a.x + c.x) / 2;
    float baseY = a.y + dir * LED_LEG_LENGTH;
    float bodyY = baseY + dir * LED_BODY_RADIUS;

    drawRect(target, a.x - LED_LEG_WIDTH / 2, std::min(a.y, baseY), LED_LEG_WIDTH, LED_LEG_LENGTH, PIN_COLOR);
    drawRect(target, c.x - LED_LEG_WIDTH / 2, std::min(c.y, baseY), LED_LEG_WIDTH, LED_LEG_LENGTH, PIN_COLOR);

    if (lit) {
        Color glow = on;
        glow.a = 70;
        drawCircle(target, centerX, bodyY, LED_GLOW_RADIUS, glow);
    }

    drawRect(target, centerX - LED_BASE_WIDTH / 2, baseY - LED_BASE_HEIGHT / 2,
             LED_BASE_WIDTH, LED_BASE_HEIGHT, lit ? on : off);
    drawCircle(target, centerX, bodyY, LED_BODY_RADIUS, lit ? on : off);
}
