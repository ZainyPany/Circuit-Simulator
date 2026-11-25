#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class PinRole {
    Signal,
    Vcc,
    Gnd
};

// Represents a terminal pin on a component
struct Pin {
    sf::Vector2f pos;      // Screen position of the pin tip
    bool isOutput;         // true = output, false = input
    PinRole role;          // Signal, Vcc (+5V), or Gnd (0V)
    bool state;            // Logic state: true = HIGH (5V), false = LOW (0V)
    bool connectedToGnd;   // True when connected to GND
    std::string tooltip;   // Label displayed on hover

    Pin(sf::Vector2f position, bool output, PinRole pinRole = PinRole::Signal,
        const std::string& desc = "")
        : pos(position),
          isOutput(output),
          role(pinRole),
          state(false),
          connectedToGnd(false),
          tooltip(desc) {}
};
