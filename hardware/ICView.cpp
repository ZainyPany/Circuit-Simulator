#include "ICView.hpp"
#include "Draw.hpp"
#include "Theme.hpp"

ICView::ICView(BreadboardView& b, Font& f, int col, string text, int pinCount)
    : board(b), font(f), firstCol(col), label(text), pins(pinCount) {}

// Calculate the hole position for a given IC pin number (1 to pinCount)
Vector2f ICView::pinPos(int pin) {
    int pinsPerSide = pins / 2;
    // Bottom row pins (1 to 7): left to right along Row F
    if (pin <= pinsPerSide) {
        return board.holePos(firstCol + pin - 1, ROW_F);
    }
    // Top row pins (8 to 14): right to left along Row E
    return board.holePos(firstCol + pins - pin, ROW_E);
}

void ICView::draw(RenderTarget& target) {
    int pinsPerSide = pins / 2;

    // 1. Draw metal pins (rectangles) and pin numbers (text)
    for (int pin = 1; pin <= pins; pin++) {
        Vector2f p = pinPos(pin);
        bool onTop = (pin > pinsPerSide);

        // Position the metal pin rectangle
        float pinY = onTop ? (p.y - PIN_OVERSHOOT) : (p.y + PIN_OVERSHOOT - PIN_HEIGHT);
        drawRect(target, p.x - PIN_WIDTH / 2.0f, pinY, PIN_WIDTH, PIN_HEIGHT, PIN_COLOR);

        // Position the pin number text
        float numberY = onTop ? (p.y - NUMBER_OFFSET) : (p.y + NUMBER_OFFSET);
        drawText(target, font, to_string(pin), NUMBER_SIZE, NUMBER_COLOR, p.x, numberY);
    }

    // 2. Draw chip body (main dark rectangle spanning across the center divider)
    float top = pinPos(pins).y + GAP_MARGIN;
    float bottom = pinPos(1).y - GAP_MARGIN;
    float left = pinPos(1).x - CHIP_OVERHANG;
    float right = pinPos(pinsPerSide).x + CHIP_OVERHANG;

    drawRect(target, left, top, right - left, bottom - top, CHIP_COLOR);

    // 3. Draw orientation dot (circle near pin 1 to show chip direction)
    drawCircle(target, left + DOT_INSET_X, pinPos(1).y - DOT_INSET_Y, DOT_RADIUS, DOT_COLOR);

    // 4. Draw chip label text (e.g., "7408 AND") in the center of the chip
    drawText(target, font, label, LABEL_SIZE, LABEL_COLOR,
             (left + right) / 2.0f + LABEL_SHIFT, (top + bottom) / 2.0f);
}
