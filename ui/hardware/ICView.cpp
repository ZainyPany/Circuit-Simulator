#include "ui/hardware/ICView.hpp"
#include "ui/hardware/DrawHelpers.hpp"
#include "ui/hardware/Theme.hpp"
using namespace std;
using namespace sf;
using namespace Theme;

ICView::ICView(const BreadboardView& board, Font& font, int firstCol, const string& label, int pins)
    : board(board), font(font), firstCol(firstCol), label(label), pins(pins) {}

Vector2f ICView::pinPos(int pin) const {
    int pinsPerSide = pins / 2;
    if (pin <= pinsPerSide) return board.holePos(firstCol + pin - 1, ROW_F);   // bottom row
    return board.holePos(firstCol + pins - pin, ROW_E);                        // top row
}

void ICView::draw(RenderTarget& target) const {
    int pinsPerSide = pins / 2;

    // pins and their numbers
    for (int pin = 1; pin <= pins; pin++) {
        Vector2f p = pinPos(pin);
        bool onTop = pin > pinsPerSide;

        float pinY = onTop ? p.y - PIN_OVERSHOOT : p.y + PIN_OVERSHOOT - PIN_HEIGHT;
        drawRect(target, p.x - PIN_WIDTH / 2, pinY, PIN_WIDTH, PIN_HEIGHT, PIN_COLOR);

        float numberY = onTop ? p.y - NUMBER_OFFSET : p.y + NUMBER_OFFSET;
        drawText(target, font, to_string(pin), NUMBER_SIZE, NUMBER_COLOR, p.x, numberY);
    }

    // body, pin 1 dot and label
    float topY = pinPos(pins).y + GAP_MARGIN;
    float bottomY = pinPos(1).y - GAP_MARGIN;
    float left = pinPos(1).x - CHIP_OVERHANG;
    float right = pinPos(pinsPerSide).x + CHIP_OVERHANG;

    drawRect(target, left, topY, right - left, bottomY - topY, CHIP_COLOR);
    drawCircle(target, left + DOT_INSET_X, pinPos(1).y - DOT_INSET_Y, DOT_RADIUS, DOT_COLOR);
    drawText(target, font, label, LABEL_SIZE, LABEL_COLOR,
             (left + right) / 2 + LABEL_SHIFT, (topY + bottomY) / 2);
}
