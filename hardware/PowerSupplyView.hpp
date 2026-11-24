#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class PowerSupplyView {
    Vector2f origin;
    Font& font;
    bool enabled;

    float panelWidth;
    float panelHeight;

    FloatRect switchBounds() const;

public:
    PowerSupplyView(Vector2f position, Font& f, bool isOn = true);

    void setEnabled(bool isOn);
    bool isEnabled() const;

    bool containsSwitch(Vector2f point) const;
    Vector2f vccTerminalPos() const;
    Vector2f gndTerminalPos() const;

    void draw(RenderTarget& target);
};
