#include "LEDView.hpp"
#include "Draw.hpp"
#include "Theme.hpp"
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
    // 1. Pick ON and OFF colors based on the LED color type
    Color on = LED_RED_ON;
    Color off = LED_RED_OFF;
    if (color == LEDColor::Green)  { on = LED_GREEN_ON;  off = LED_GREEN_OFF;  }
    if (color == LEDColor::Yellow) { on = LED_YELLOW_ON; off = LED_YELLOW_OFF; }
    if (color == LEDColor::Blue)   { on = LED_BLUE_ON;   off = LED_BLUE_OFF;   }

    Color currentColor = lit ? on : off;

    // 2. Compute positions for legs and body
    Vector2f a = anodePos();
    Vector2f c = cathodePos();
    // Direction: point upwards (-1) if in top rows A-E, downwards (+1) if in bottom rows F-J
    float dir = (row <= ROW_E) ? -1.0f : 1.0f;
    float centerX = (a.x + c.x) / 2.0f;
    float baseY = a.y + dir * LED_LEG_LENGTH;
    float bodyY = baseY + dir * LED_BODY_RADIUS;

    // 3. Draw two metal legs (thin rectangles)
    drawRect(target, a.x - LED_LEG_WIDTH / 2.0f, std::min(a.y, baseY), LED_LEG_WIDTH, LED_LEG_LENGTH, PIN_COLOR);
    drawRect(target, c.x - LED_LEG_WIDTH / 2.0f, std::min(c.y, baseY), LED_LEG_WIDTH, LED_LEG_LENGTH, PIN_COLOR);

    // 4. Draw soft glow circle if the LED is lit
    if (lit) {
        Color glow = on;
        glow.a = 70; // Translucent glow
        drawCircle(target, centerX, bodyY, LED_GLOW_RADIUS, glow);
    }

    // 5. Draw LED base rim (rectangle)
    drawRect(target, centerX - LED_BASE_WIDTH / 2.0f, baseY - LED_BASE_HEIGHT / 2.0f,
             LED_BASE_WIDTH, LED_BASE_HEIGHT, currentColor);

    // 6. Draw LED dome bulb (circle)
    drawCircle(target, centerX, bodyY, LED_BODY_RADIUS, currentColor);
}
