#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class PinRole {
    Signal,
    Vcc,
    Gnd
};

// A clickable leg (pin) on any component
struct Pin {
    sf::Vector2f pos;      // Screen position of the pin tip
    bool isOutput;         // true = drives signal out, false = receives signal in
    PinRole role;          // Signal, Vcc (+5V), or Gnd (0V)
    bool state;            // Current binary logic value (true = 1 / 5V, false = 0 / 0V)
    bool connectedToGnd;   // True when connected to an active GND terminal
    std::string tooltip;   // Human-readable name shown when hovering

    Pin(sf::Vector2f position, bool output, PinRole pinRole = PinRole::Signal,
        const std::string& desc = "")
        : pos(position),
          isOutput(output),
          role(pinRole),
          state(false),
          connectedToGnd(false),
          tooltip(desc) {}
};
