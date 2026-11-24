#include "Components.hpp"
#include "DrawHelpers.hpp"

// ============================================================================
// PowerSupplyUnit
// ============================================================================

PowerSupplyUnit::PowerSupplyUnit(sf::Vector2f position)
    : Component("5V POWER", position, {130.0f, 100.0f}), enabled(true) {
    // Pin 0: +5V VCC output terminal
    pins.push_back(Pin({pos.x + size.x + 16.0f, pos.y + 30.0f},
                       true, PinRole::Vcc, "5V Supply | +5V VCC Terminal"));
    // Pin 1: GND (0V) output terminal
    pins.push_back(Pin({pos.x + size.x + 16.0f, pos.y + 72.0f},
                       true, PinRole::Gnd, "5V Supply | GND (0V) Terminal"));
}

void PowerSupplyUnit::toggle() {
    enabled = !enabled;
}

void PowerSupplyUnit::evaluate(bool powerOn) {
    bool active = (enabled && powerOn);
    pins[0].state = active;
    pins[1].state = false;
    pins[1].connectedToGnd = active;
}

void PowerSupplyUnit::draw(sf::RenderWindow& window, const sf::Font& font) const {
    // Terminal stubs
    drawThickLine(window, {pos.x + size.x, pins[0].pos.y}, pins[0].pos, 4.0f, sf::Color(220, 65, 65));
    drawThickLine(window, {pos.x + size.x, pins[1].pos.y}, pins[1].pos, 4.0f, sf::Color(110, 125, 150));

    // Box
    sf::RectangleShape body(size);
    body.setPosition(pos);
    body.setFillColor(enabled ? sf::Color(35, 115, 65) : sf::Color(125, 45, 45));
    body.setOutlineThickness(2.0f);
    body.setOutlineColor(sf::Color(215, 220, 230));
    window.draw(body);

    drawCenteredText(window, font, "5V SUPPLY", 13, sf::Color::White,
                     pos.x + size.x / 2.0f - 10.0f, pos.y + 24.0f);
    drawCenteredText(window, font, enabled ? "ON (5.0V)" : "OFF (0.0V)", 13,
                     sf::Color(240, 245, 250), pos.x + size.x / 2.0f - 10.0f, pos.y + 52.0f);
    drawCenteredText(window, font, "(click to toggle)", 10,
                     sf::Color(210, 220, 225), pos.x + size.x / 2.0f - 10.0f, pos.y + 80.0f);

    // Terminal labels inside right edge
    drawCenteredText(window, font, "+5V", 10, sf::Color(255, 180, 180), pos.x + size.x - 16.0f, pins[0].pos.y);
    drawCenteredText(window, font, "GND", 10, sf::Color(190, 205, 230), pos.x + size.x - 16.0f, pins[1].pos.y);
}

// ============================================================================
// PushButton
// ============================================================================

PushButton::PushButton(const std::string& name, sf::Vector2f position)
    : Component(name, position, {105.0f, 54.0f}), pressed(false) {
    pins.push_back(Pin({pos.x + size.x + 16.0f, pos.y + size.y / 2.0f},
                       true, PinRole::Signal, name + " | Output Pin"));
}

void PushButton::toggle() {
    pressed = !pressed;
}

void PushButton::evaluate(bool powerOn) {
    pins[0].state = (powerOn && pressed);
}

void PushButton::draw(sf::RenderWindow& window, const sf::Font& font) const {
    drawThickLine(window, {pos.x + size.x, pos.y + size.y / 2.0f},
                  pins[0].pos, 4.0f, sf::Color(180, 185, 195));

    sf::RectangleShape body(size);
    body.setPosition(pos);
    body.setFillColor(pressed ? sf::Color(235, 145, 45) : sf::Color(62, 70, 88));
    body.setOutlineThickness(2.0f);
    body.setOutlineColor(sf::Color(210, 215, 225));
    window.draw(body);

    drawCenteredText(window, font, label, 14, sf::Color::White,
                     pos.x + size.x / 2.0f, pos.y + 18.0f);
    drawCenteredText(window, font, pressed ? "ON (1)" : "OFF (0)", 12,
                     sf::Color(240, 240, 240), pos.x + size.x / 2.0f, pos.y + 38.0f);
}

// ============================================================================
// GateIC — Authentic 14-Pin DIP Packages
// ============================================================================

GateIC::GateIC(GateType gateType, const std::string& name, sf::Vector2f position)
    : Component(name, position, {220.0f, 68.0f}), type(gateType), powered(false) {
    // 1. Define real datasheet gate pin mappings
    if (type == GateType::IC7404_NOT) {
        // 7404 Hex Inverter: 6 NOT gates
        gates = {
            {1, 0, 2}, {3, 0, 4}, {5, 0, 6},
            {9, 0, 8}, {11, 0, 10}, {13, 0, 12}
        };
    } else if (type == GateType::IC7402_NOR) {
        // 7402 Quad 2-Input NOR (outputs on pins 1, 4, 10, 13)
        gates = {
            {2, 3, 1}, {5, 6, 4}, {8, 9, 10}, {11, 12, 13}
        };
    } else {
        // 7408 Quad AND & 7486 Quad XOR (outputs on pins 3, 6, 8, 11)
        gates = {
            {1, 2, 3}, {4, 5, 6}, {9, 10, 8}, {12, 13, 11}
        };
    }

    // Helper to build tooltip & check if a 1-based pin is an output
    auto isPinOutput = [&](int pinNum) {
        for (std::size_t i = 0; i < gates.size(); ++i) {
            if (gates[i].out == pinNum) return true;
        }
        return false;
    };

    auto makePinTooltip = [&](int pinNum) -> std::string {
        if (pinNum == 14) return name + " | Pin 14 (VCC +5V)";
        if (pinNum == 7)  return name + " | Pin 7 (GND 0V)";
        for (std::size_t i = 0; i < gates.size(); ++i) {
            std::string g = std::to_string(i + 1);
            if (gates[i].in1 == pinNum) return name + " | Pin " + std::to_string(pinNum) + " (Gate " + g + " In A)";
            if (gates[i].in2 == pinNum) return name + " | Pin " + std::to_string(pinNum) + " (Gate " + g + " In B)";
            if (gates[i].out == pinNum) return name + " | Pin " + std::to_string(pinNum) + " (Gate " + g + " Out Y)";
        }
        return name + " | Pin " + std::to_string(pinNum);
    };

    // 2. Create all 14 pins in order (pins[0] = Pin 1 ... pins[13] = Pin 14)
    //    Bottom row: Pins 1..7  left to right
    //    Top row:    Pins 14..8 left to right
    const float startX = pos.x + 20.0f;
    const float spacing = 30.0f;
    const float legLength = 18.0f;

    for (int pinNum = 1; pinNum <= 14; ++pinNum) {
        sf::Vector2f pinPos;
        if (pinNum <= 7) {
            pinPos.x = startX + (pinNum - 1) * spacing;
            pinPos.y = pos.y + size.y + legLength;
        } else {
            pinPos.x = startX + (14 - pinNum) * spacing;
            pinPos.y = pos.y - legLength;
        }

        PinRole role = PinRole::Signal;
        if (pinNum == 14) role = PinRole::Vcc;
        if (pinNum == 7)  role = PinRole::Gnd;

        bool out = isPinOutput(pinNum);
        pins.push_back(Pin(pinPos, out, role, makePinTooltip(pinNum)));
    }
}

void GateIC::evaluate(bool powerOn) {
    // Real IC power requirement: Pin 14 (index 13) = +5V VCC, Pin 7 (index 6) = GND
    bool hasVcc = pins[13].state;
    bool hasGnd = pins[6].connectedToGnd;
    powered = (powerOn && hasVcc && hasGnd);

    for (std::size_t i = 0; i < gates.size(); ++i) {
        const InternalGate& g = gates[i];
        Pin& outPin = pins[static_cast<std::size_t>(g.out - 1)];

        if (!powered) {
            outPin.state = false;
            continue;
        }

        bool a = pins[static_cast<std::size_t>(g.in1 - 1)].state;
        bool b = (g.in2 > 0) ? pins[static_cast<std::size_t>(g.in2 - 1)].state : false;

        if (type == GateType::IC7404_NOT) {
            outPin.state = !a;
        } else if (type == GateType::IC7408_AND) {
            outPin.state = (a && b);
        } else if (type == GateType::IC7402_NOR) {
            outPin.state = !(a || b);
        } else if (type == GateType::IC7486_XOR) {
            outPin.state = (a != b);
        }
    }
}

void GateIC::draw(sf::RenderWindow& window, const sf::Font& font) const {
    // Draw all 14 metallic leg stubs
    for (int pinNum = 1; pinNum <= 14; ++pinNum) {
        const Pin& p = pins[static_cast<std::size_t>(pinNum - 1)];
        float bodyEdgeY = (pinNum <= 7) ? (pos.y + size.y) : pos.y;
        drawThickLine(window, {p.pos.x, bodyEdgeY}, p.pos, 4.0f, sf::Color(180, 185, 195));
    }

    // Dark DIP IC package body
    sf::RectangleShape chip(size);
    chip.setPosition(pos);
    chip.setFillColor(sf::Color(38, 42, 52));
    chip.setOutlineThickness(2.0f);
    chip.setOutlineColor(sf::Color(145, 155, 175));
    window.draw(chip);

    // Pin-1 orientation notch on left edge
    sf::CircleShape notch(8.0f);
    notch.setOrigin({8.0f, 8.0f});
    notch.setPosition({pos.x + 2.0f, pos.y + size.y / 2.0f});
    notch.setFillColor(sf::Color(22, 25, 32));
    window.draw(notch);

    // Pin-1 dot near bottom-left
    sf::CircleShape pin1Dot(3.0f);
    pin1Dot.setOrigin({3.0f, 3.0f});
    pin1Dot.setPosition({pos.x + 12.0f, pos.y + size.y - 12.0f});
    pin1Dot.setFillColor(sf::Color(120, 128, 145));
    window.draw(pin1Dot);

    // Power status indicator dot on right edge (green = VCC + GND connected)
    sf::CircleShape pwrDot(4.5f);
    pwrDot.setOrigin({4.5f, 4.5f});
    pwrDot.setPosition({pos.x + size.x - 14.0f, pos.y + size.y / 2.0f});
    pwrDot.setFillColor(powered ? sf::Color(70, 240, 110) : sf::Color(135, 45, 45));
    window.draw(pwrDot);

    // IC model label
    drawCenteredText(window, font, label, 15, sf::Color(235, 240, 250),
                     pos.x + size.x / 2.0f, pos.y + size.y / 2.0f);

    // Pin numbers inside top & bottom edges of IC
    for (int pinNum = 1; pinNum <= 14; ++pinNum) {
        const Pin& p = pins[static_cast<std::size_t>(pinNum - 1)];
        float numY = (pinNum <= 7) ? (pos.y + size.y - 10.0f) : (pos.y + 10.0f);

        std::string tag = std::to_string(pinNum);
        sf::Color tagColor(175, 182, 198);
        if (pinNum == 14) { tag = "VCC"; tagColor = sf::Color(245, 110, 110); }
        if (pinNum == 7)  { tag = "GND"; tagColor = sf::Color(125, 175, 245); }

        drawCenteredText(window, font, tag, 9, tagColor, p.pos.x, numY);
    }
}

// ============================================================================
// LED
// ============================================================================

LED::LED(const std::string& name, sf::Vector2f position,
         sf::Color litColor, sf::Color dimColor)
    : Component(name, position, {64.0f, 64.0f}),
      onColor(litColor), offColor(dimColor), lit(false) {
    pins.push_back(Pin({pos.x - 16.0f, pos.y + size.y / 2.0f},
                       false, PinRole::Signal, name + " | Input Pin"));
}

void LED::evaluate(bool powerOn) {
    lit = (powerOn && pins[0].state);
}

void LED::draw(sf::RenderWindow& window, const sf::Font& font) const {
    sf::Vector2f center(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f);

    drawThickLine(window, {pos.x + 8.0f, center.y}, pins[0].pos, 4.0f, sf::Color(180, 185, 195));

    if (lit) {
        sf::Color glow = onColor;
        glow.a = 80;
        sf::CircleShape halo(34.0f);
        halo.setOrigin({34.0f, 34.0f});
        halo.setPosition(center);
        halo.setFillColor(glow);
        window.draw(halo);
    }

    sf::CircleShape bulb(22.0f);
    bulb.setOrigin({22.0f, 22.0f});
    bulb.setPosition(center);
    bulb.setFillColor(lit ? onColor : offColor);
    bulb.setOutlineThickness(2.0f);
    bulb.setOutlineColor(sf::Color(220, 225, 235));
    window.draw(bulb);

    drawCenteredText(window, font, label, 12, sf::Color::White,
                     center.x, pos.y + size.y + 14.0f);
}
