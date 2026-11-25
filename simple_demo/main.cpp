#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "Component.hpp"
#include "Components.hpp"
#include "DrawHelpers.hpp"
#include "Pin.hpp"
#include "Wire.hpp"

// Simulates circuit by resetting inputs, evaluating components, and propagating wire states
static void simulateCircuit(std::vector<Component*>& components,
                            std::vector<Wire>& wires,
                            bool powerOn) {
    for (std::size_t i = 0; i < components.size(); ++i) {
        for (std::size_t p = 0; p < components[i]->pins.size(); ++p) {
            if (!components[i]->pins[p].isOutput) {
                components[i]->pins[p].state = false;
                components[i]->pins[p].connectedToGnd = false;
            }
        }
    }

    // Multiple passes allow signals to settle through chained gates
    for (int pass = 0; pass < 8; ++pass) {
        for (std::size_t i = 0; i < components.size(); ++i) {
            components[i]->evaluate(powerOn);
        }
        for (std::size_t w = 0; w < wires.size(); ++w) {
            wires[w].propagate();
        }
    }
}

// Finds the closest pin to the mouse position within detection radius
static Pin* findPinNear(std::vector<Component*>& components, sf::Vector2f mousePos) {
    for (std::size_t i = 0; i < components.size(); ++i) {
        for (std::size_t p = 0; p < components[i]->pins.size(); ++p) {
            if (distanceBetween(mousePos, components[i]->pins[p].pos) <= 12.0f) {
                return &components[i]->pins[p];
            }
        }
    }
    return nullptr;
}

// Checks if two pins can legally be connected by a wire
static bool canConnect(const Pin* a, const Pin* b) {
    if (a == nullptr || b == nullptr || a == b) return false;
    if (a->isOutput == b->isOutput) return false;

    const Pin* outPin = a->isOutput ? a : b;
    const Pin* inPin  = a->isOutput ? b : a;

    if (outPin->role == PinRole::Vcc && inPin->role == PinRole::Gnd) return false;
    if (outPin->role == PinRole::Gnd && inPin->role == PinRole::Vcc) return false;
    if (outPin->role == PinRole::Signal && inPin->role != PinRole::Signal) return false;

    return true;
}

int main() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode({1120u, 640u}),
                            "Simple Digital Logic Circuit Simulator (14-Pin DIP ICs)",
                            sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    sf::Font font;
    bool fontOk = font.openFromFile("C:/Windows/Fonts/arial.ttf") ||
                  font.openFromFile("C:/Windows/Fonts/segoeui.ttf") ||
                  font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    if (!fontOk) {
        std::cerr << "Error: Could not load system font.\n";
        return 1;
    }

    PowerSupplyUnit supply({36.0f, 88.0f});

    PushButton btnA("BTN A", {36.0f, 250.0f});
    PushButton btnB("BTN B", {36.0f, 360.0f});
    PushButton btnC("BTN C", {36.0f, 470.0f});

    GateIC icAnd(GateType::IC7408_AND, "7408 AND", {280.0f, 155.0f});
    GateIC icNor(GateType::IC7402_NOR, "7402 NOR", {575.0f, 155.0f});
    GateIC icNot(GateType::IC7404_NOT, "7404 NOT", {280.0f, 395.0f});
    GateIC icXor(GateType::IC7486_XOR, "7486 XOR", {575.0f, 395.0f});

    LED ledRed  ("RED LED",   {940.0f, 130.0f}, sf::Color(255, 70, 70),  sf::Color(115, 35, 35)); // Bright red / dark red
    LED ledGreen("GREEN LED", {940.0f, 280.0f}, sf::Color(70, 240, 110), sf::Color(30, 95, 45));  // Bright green / dark green
    LED ledBlue ("BLUE LED",  {940.0f, 430.0f}, sf::Color(80, 165, 255), sf::Color(35, 65, 115)); // Bright blue / dark blue

    std::vector<Component*> components = {
        &supply,
        &btnA, &btnB, &btnC,
        &icAnd, &icNor, &icNot, &icXor,
        &ledRed, &ledGreen, &ledBlue
    };
    std::vector<PushButton*> buttons = {&btnA, &btnB, &btnC};
    std::vector<GateIC*> ics = {&icAnd, &icNor, &icNot, &icXor};
    std::vector<Wire> wires;

    sf::FloatRect autoPowerBtn({20.0f, 12.0f},  {155.0f, 34.0f});
    sf::FloatRect undoBtn     ({190.0f, 12.0f}, {120.0f, 34.0f});
    sf::FloatRect clearBtn    ({325.0f, 12.0f}, {130.0f, 34.0f});

    Pin* anchoredPin = nullptr;
    Pin* hoveredPin  = nullptr;
    sf::Vector2f mousePos(0.0f, 0.0f);

    simulateCircuit(components, wires, supply.enabled);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (auto moved = event->getIf<sf::Event::MouseMoved>()) {
                mousePos = window.mapPixelToCoords(moved->position);
                hoveredPin = findPinNear(components, mousePos);
            }

            if (auto key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) {
                    anchoredPin = nullptr;
                } else if (key->code == sf::Keyboard::Key::Backspace ||
                           key->code == sf::Keyboard::Key::Z) {
                    if (!wires.empty()) {
                        wires.pop_back();
                        simulateCircuit(components, wires, supply.enabled);
                    }
                } else if (key->code == sf::Keyboard::Key::C) {
                    wires.clear();
                    anchoredPin = nullptr;
                    simulateCircuit(components, wires, supply.enabled);
                }
            }

            if (auto click = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f clickPos = window.mapPixelToCoords(click->position);

                if (click->button == sf::Mouse::Button::Right) {
                    anchoredPin = nullptr;
                    continue;
                }
                if (click->button != sf::Mouse::Button::Left) continue;

                if (autoPowerBtn.contains(clickPos)) {
                    for (std::size_t i = 0; i < ics.size(); ++i) {
                        wires.push_back(Wire{&supply.pins[0], &ics[i]->pins[13]});
                        wires.push_back(Wire{&supply.pins[1], &ics[i]->pins[6]});
                    }
                    simulateCircuit(components, wires, supply.enabled);
                    continue;
                }
                if (undoBtn.contains(clickPos)) {
                    anchoredPin = nullptr;
                    if (!wires.empty()) wires.pop_back();
                    simulateCircuit(components, wires, supply.enabled);
                    continue;
                }
                if (clearBtn.contains(clickPos)) {
                    anchoredPin = nullptr;
                    wires.clear();
                    simulateCircuit(components, wires, supply.enabled);
                    continue;
                }

                Pin* clickedPin = findPinNear(components, clickPos);
                if (clickedPin != nullptr) {
                    if (anchoredPin == nullptr) {
                        anchoredPin = clickedPin;
                    } else {
                        if (canConnect(anchoredPin, clickedPin)) {
                            Pin* outPin = anchoredPin->isOutput ? anchoredPin : clickedPin;
                            Pin* inPin  = anchoredPin->isOutput ? clickedPin  : anchoredPin;
                            wires.push_back(Wire{outPin, inPin});
                            simulateCircuit(components, wires, supply.enabled);
                        }
                        anchoredPin = nullptr;
                    }
                    continue;
                }

                if (supply.containsBody(clickPos)) {
                    supply.toggle();
                    simulateCircuit(components, wires, supply.enabled);
                    continue;
                }

                bool hitButton = false;
                for (std::size_t i = 0; i < buttons.size(); ++i) {
                    if (buttons[i]->containsBody(clickPos)) {
                        buttons[i]->toggle();
                        simulateCircuit(components, wires, supply.enabled);
                        hitButton = true;
                        break;
                    }
                }

                if (!hitButton) {
                    anchoredPin = nullptr;
                }
            }
        }

        window.clear(sf::Color(30, 34, 44)); // Dark slate background

        sf::RectangleShape topBar({1120.0f, 58.0f});
        topBar.setFillColor(sf::Color(20, 23, 32)); // Dark toolbar background
        window.draw(topBar);

        auto drawBtn = [&](sf::FloatRect rect, const std::string& text, sf::Color bg) {
            sf::RectangleShape box(rect.size);
            box.setPosition(rect.position);
            box.setFillColor(bg);
            box.setOutlineThickness(1.5f);
            box.setOutlineColor(sf::Color(175, 185, 205)); // Gray outline
            window.draw(box);
            drawCenteredText(window, font, text, 13, sf::Color::White,
                             rect.position.x + rect.size.x / 2.0f,
                             rect.position.y + rect.size.y / 2.0f);
        };

        drawBtn(autoPowerBtn, "Auto-Power ICs", sf::Color(45, 110, 165)); // Blue button
        drawBtn(undoBtn, "Undo Wire", sf::Color(55, 62, 80));              // Slate button
        drawBtn(clearBtn, "Clear Wires", sf::Color(55, 62, 80));           // Slate button

        drawCenteredText(window, font,
                         "Wire Pin 14 (VCC) to +5V & Pin 7 (GND) to GND  |  Gold ring = Output leg",
                         12, sf::Color(175, 185, 205), 795.0f, 29.0f); // Light gray text

        for (std::size_t i = 0; i < wires.size(); ++i) {
            wires[i].draw(window);
        }

        if (anchoredPin != nullptr) {
            drawThickLine(window, anchoredPin->pos, mousePos, 3.5f, sf::Color(255, 220, 70)); // Yellow wire preview
        }

        for (std::size_t i = 0; i < components.size(); ++i) {
            components[i]->draw(window, font);
            components[i]->drawPins(window, anchoredPin);
        }

        sf::RectangleShape bottomBar({1120.0f, 40.0f});
        bottomBar.setPosition({0.0f, 600.0f});
        bottomBar.setFillColor(sf::Color(20, 23, 32)); // Dark toolbar background
        window.draw(bottomBar);

        std::string statusText = "Hover over any IC leg to see its pin function  |  Click any leg to start/finish a wire  |  Right-click / Esc to cancel";
        if (hoveredPin != nullptr) {
            std::string level = hoveredPin->state ? "HIGH (1 / 5V)"
                              : (hoveredPin->connectedToGnd ? "GND (0V)" : "LOW (0)");
            statusText = "Pin Probe:  " + hoveredPin->tooltip + "  |  State: " + level;
        }
        drawCenteredText(window, font, statusText, 13, sf::Color(225, 232, 245), 560.0f, 620.0f); // Off-white text

        window.display();
    }

    return 0;
}
