#include "hardware/ICView.hpp"
#include "hardware/Draw.hpp"
#include "hardware/Theme.hpp"

ICView::ICView(BreadboardView& b, Font& f, int col, string text, int pinCount)
    : board(b), font(f), firstCol(col), label(text), pins(pinCount) {}

Vector2f ICView::pinPos(int pin) {
    int pinsPerSide = pins / 2;
    if (pin <= pinsPerSide) return board.holePos(firstCol + pin - 1, ROW_F);   // bottom row
    return board.holePos(firstCol + pins - pin, ROW_E);                        // top row
}

void ICView::draw(RenderTarget& target) {
    int pinsPerSide = pins / 2;

    // pins and their numbers
    for (int pin = 1; pin <= pins; pin++) {
        Vector2f p = pinPos(pin);
        bool onTop = pin > pinsPerSide;

        float pinY = p.y + PIN_OVERSHOOT - PIN_HEIGHT;
        if (onTop) pinY = p.y - PIN_OVERSHOOT;
        drawRect(target, p.x - PIN_WIDTH / 2, pinY, PIN_WIDTH, PIN_HEIGHT, PIN_COLOR);

        float numberY = p.y + NUMBER_OFFSET;
        if (onTop) numberY = p.y - NUMBER_OFFSET;
        drawText(target, font, to_string(pin), NUMBER_SIZE, NUMBER_COLOR, p.x, numberY);
    }

    // body, pin 1 dot and label
    float top = pinPos(pins).y + GAP_MARGIN;
    float bottom = pinPos(1).y - GAP_MARGIN;
    float left = pinPos(1).x - CHIP_OVERHANG;
    float right = pinPos(pinsPerSide).x + CHIP_OVERHANG;

    drawRect(target, left, top, right - left, bottom - top, CHIP_COLOR);
    drawCircle(target, left + DOT_INSET_X, pinPos(1).y - DOT_INSET_Y, DOT_RADIUS, DOT_COLOR);
    drawText(target, font, label, LABEL_SIZE, LABEL_COLOR,
             (left + right) / 2 + LABEL_SHIFT, (top + bottom) / 2);
}
