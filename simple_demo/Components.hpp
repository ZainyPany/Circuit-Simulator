#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Component.hpp"

// 5V DC power supply with +5V and GND output terminals
class PowerSupplyUnit : public Component {
public:
    bool enabled;

    explicit PowerSupplyUnit(sf::Vector2f position);

    void toggle();
    void evaluate(bool powerOn) override;
    void draw(sf::RenderWindow& window, const sf::Font& font) const override;
};

// Push button that toggles between logic 0 and 1
class PushButton : public Component {
public:
    bool pressed;

    PushButton(const std::string& name, sf::Vector2f position);

    void toggle();
    void evaluate(bool powerOn) override;
    void draw(sf::RenderWindow& window, const sf::Font& font) const override;
};

// 14-pin DIP TTL logic IC
enum class GateType {
    IC7408_AND,
    IC7402_NOR,
    IC7404_NOT,
    IC7486_XOR
};

struct InternalGate {
    int in1;  // 1-based pin number for input 1
    int in2;  // 1-based pin number for input 2 (0 for NOT gate)
    int out;  // 1-based pin number for output
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

// LED indicator
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
