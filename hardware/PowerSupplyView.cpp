#include "hardware/PowerSupplyView.hpp"
#include "hardware/Draw.hpp"
#include "hardware/Theme.hpp"

PowerSupplyView::PowerSupplyView(Vector2f position, Font& f, bool isOn)
    : origin(position), font(f), enabled(isOn), panelWidth(190.0f), panelHeight(250.0f) {}

void PowerSupplyView::setEnabled(bool isOn) {
    enabled = isOn;
}

bool PowerSupplyView::isEnabled() const {
    return enabled;
}

FloatRect PowerSupplyView::switchBounds() const {
    return FloatRect({origin.x + 30.0f, origin.y + 115.0f}, {130.0f, 38.0f});
}

bool PowerSupplyView::containsSwitch(Vector2f point) const {
    return switchBounds().contains(point);
}

Vector2f PowerSupplyView::vccTerminalPos() const {
    return {origin.x + panelWidth - 18.0f, origin.y + 190.0f};
}

Vector2f PowerSupplyView::gndTerminalPos() const {
    return {origin.x + panelWidth - 18.0f, origin.y + 224.0f};
}

void PowerSupplyView::draw(RenderTarget& target) {
    // Outer chassis
    drawRect(target, origin.x, origin.y, panelWidth, panelHeight, Color(34, 38, 50));
    drawRect(target, origin.x + 3.0f, origin.y + 3.0f, panelWidth - 6.0f, panelHeight - 6.0f, Color(42, 46, 60));

    // Header bar
    drawRect(target, origin.x + 3.0f, origin.y + 3.0f, panelWidth - 6.0f, 30.0f, Color(28, 32, 42));
    drawText(target, font, "5V DC SUPPLY", 13, Color(230, 234, 242), origin.x + panelWidth * 0.5f, origin.y + 18.0f);

    // Digital voltage readout screen
    drawRect(target, origin.x + 18.0f, origin.y + 46.0f, panelWidth - 36.0f, 50.0f, Color(18, 22, 28));
    Color readoutColor = enabled ? Color(80, 240, 120) : Color(150, 65, 65);
    drawText(target, font, enabled ? "5.00 V" : "0.00 V", 22, readoutColor,
             origin.x + panelWidth * 0.5f - 10.0f, origin.y + 71.0f);

    // Power status indicator LED
    Vector2f ledCenter(origin.x + panelWidth - 34.0f, origin.y + 71.0f);
    if (enabled) {
        drawCircle(target, ledCenter.x, ledCenter.y, 9.0f, Color(80, 240, 110, 70));
    }
    drawCircle(target, ledCenter.x, ledCenter.y, 5.5f, enabled ? LED_GREEN_ON : LED_RED_OFF);

    // Clickable power toggle button
    FloatRect sw = switchBounds();
    Color btnBg = enabled ? Color(42, 145, 78) : Color(155, 52, 52);
    drawRect(target, sw.position.x, sw.position.y, sw.size.x, sw.size.y, Color(20, 24, 32));
    drawRect(target, sw.position.x + 2.0f, sw.position.y + 2.0f, sw.size.x - 4.0f, sw.size.y - 4.0f, btnBg);
    drawText(target, font, enabled ? "POWER: ON" : "POWER: OFF", 13, Color(250, 250, 250),
             sw.position.x + sw.size.x * 0.5f, sw.position.y + sw.size.y * 0.5f);

    // Output terminals (+5V red, GND black)
    Vector2f vcc = vccTerminalPos();
    Vector2f gnd = gndTerminalPos();

    drawText(target, font, "+5V OUT", 11, Color(235, 110, 110), origin.x + 56.0f, vcc.y);
    drawCircle(target, vcc.x, vcc.y, 9.0f, Color(210, 55, 55));
    drawCircle(target, vcc.x, vcc.y, 4.0f, Color(240, 220, 140));

    drawText(target, font, "GND (0V)", 11, Color(170, 180, 195), origin.x + 56.0f, gnd.y);
    drawCircle(target, gnd.x, gnd.y, 9.0f, Color(24, 26, 30));
    drawCircle(target, gnd.x, gnd.y, 4.0f, Color(180, 185, 195));
}
