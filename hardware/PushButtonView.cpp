#include "hardware/PushButtonView.hpp"
#include "hardware/Draw.hpp"
#include "hardware/Theme.hpp"

PushButtonView::PushButtonView(BreadboardView& b, Font& f, int column, int rowIndex,
                               const string& text, bool isPressed)
    : board(b), font(f), col(column), row(rowIndex), label(text), pressed(isPressed) {}

void PushButtonView::setPressed(bool isPressed) {
    pressed = isPressed;
}

bool PushButtonView::isPressed() const {
    return pressed;
}

Vector2f PushButtonView::leftPinPos() const {
    return board.holePos(col, row);
}

Vector2f PushButtonView::rightPinPos() const {
    return board.holePos(col + 1, row);
}

Vector2f PushButtonView::centerPos() const {
    Vector2f l = leftPinPos();
    Vector2f r = rightPinPos();
    return {(l.x + r.x) * 0.5f, l.y};
}

bool PushButtonView::contains(Vector2f point) const {
    Vector2f c = centerPos();
    FloatRect box({c.x - 16.0f, c.y - 16.0f}, {32.0f, 32.0f});
    return box.contains(point);
}

void PushButtonView::draw(RenderTarget& target) {
    Vector2f l = leftPinPos();
    Vector2f r = rightPinPos();
    Vector2f c = centerPos();

    // Metallic legs into breadboard holes
    drawRect(target, l.x - 2.0f, l.y - 11.0f, 4.0f, 22.0f, PIN_COLOR);
    drawRect(target, r.x - 2.0f, r.y - 11.0f, 4.0f, 22.0f, PIN_COLOR);

    // Tactile switch outer body
    float bodySize = 26.0f;
    drawRect(target, c.x - bodySize * 0.5f, c.y - bodySize * 0.5f, bodySize, bodySize, Color(55, 58, 66));
    drawRect(target, c.x - bodySize * 0.5f + 2.0f, c.y - bodySize * 0.5f + 2.0f,
             bodySize - 4.0f, bodySize - 4.0f, Color(185, 188, 195));

    // Corner rivets
    float rv = 8.5f;
    drawCircle(target, c.x - rv, c.y - rv, 1.5f, Color(75, 78, 86));
    drawCircle(target, c.x + rv, c.y - rv, 1.5f, Color(75, 78, 86));
    drawCircle(target, c.x - rv, c.y + rv, 1.5f, Color(75, 78, 86));
    drawCircle(target, c.x + rv, c.y + rv, 1.5f, Color(75, 78, 86));

    // Push-button cap
    float capRadius = pressed ? 7.0f : 8.0f;
    Color capColor = pressed ? Color(245, 155, 45) : Color(40, 44, 52);
    Color capRim = pressed ? Color(255, 200, 95) : Color(70, 74, 84);
    drawCircle(target, c.x, c.y, capRadius + 1.2f, capRim);
    drawCircle(target, c.x, c.y, capRadius, capColor);

    // Label and active logic state
    drawText(target, font, label, 9, Color(40, 44, 52), c.x, c.y - 22.0f);
    drawText(target, font, pressed ? "1" : "0", 9,
             pressed ? Color(25, 25, 25) : Color(220, 224, 232), c.x, c.y);
}
