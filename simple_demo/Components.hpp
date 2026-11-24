#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Component.hpp"

// ----------------------------------------------------------------------------
// 5V DC Power Supply (has +5V and GND output terminals to power the ICs)
// ----------------------------------------------------------------------------
class PowerSupplyUnit : public Component {
public:
    bool enabled;

    explicit PowerSupplyUnit(sf::Vector2f position);

    void toggle();
    void evaluate(bool powerOn) override;
    void draw(sf::RenderWindow& window, const sf::Font& font) const override;
};

// ----------------------------------------------------------------------------
// Push Button (click body to toggle 0 / 1; has 1 Output leg)
// ----------------------------------------------------------------------------
class PushButton : public Component {
public:
    bool pressed;

    PushButton(const std::string& name, sf::Vector2f position);

    void toggle();
    void evaluate(bool powerOn) override;
    void draw(sf::RenderWindow& window, const sf::Font& font) const override;
};

// ----------------------------------------------------------------------------
// Original 14-Pin DIP TTL Logic ICs: 7408 (AND), 7402 (NOR), 7404 (NOT), 7486 (XOR)
//   - Bottom row (left to right): Pins 1..7   (Pin 7  = GND)
//   - Top row    (left to right): Pins 14..8  (Pin 14 = VCC +5V)
// ----------------------------------------------------------------------------
enum class GateType {
    IC7408_AND,
    IC7402_NOR,
    IC7404_NOT,
    IC7486_XOR
};

struct InternalGate {
    int in1;  // 1-based pin number
    int in2;  // 1-based pin number (0 for 1-input NOT gate)
    int out;  // 1-based pin number
};

class GateIC : public Component {
public:
    GateType type;
    bool powered;
    std::vector<InternalGate> gates;

    GateIC(GateType gateType, const std::string& name, sf::Vector2f position);

    void evaluate(bool powerOn) override;
    void draw(sf::RenderWindow& window, const sf::Font& font) const override;
};

// ----------------------------------------------------------------------------
// LED (Red, Green, Blue; has 1 Input leg)
// ----------------------------------------------------------------------------
class LED : public Component {
public:
    sf::Color onColor;
    sf::Color offColor;
    bool lit;

    LED(const std::string& name, sf::Vector2f position,
        sf::Color litColor, sf::Color dimColor);

    void evaluate(bool powerOn) override;
    void draw(sf::RenderWindow& window, const sf::Font& font) const override;
};
