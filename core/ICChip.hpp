#pragma once
#include <memory>
#include <string>
#include <vector>
#include "gates/LogicGate.hpp"

enum class ICType {
    IC7408_AND,
    IC7404_NOT,
    IC7432_OR,
    IC7486_XOR
};

struct GatePinMap {
    int in1;
    int in2; // 0 for single-input gates (7404 NOT)
    int out;
};

// Models a standard 14-pin DIP 74xx TTL Integrated Circuit composed of LogicGate instances.
// Pin 14 = VCC (+5V), Pin 7 = GND (0V).
class ICChip {
private:
    ICType type;
    std::string label;
    int firstCol;
    bool powered;
    std::vector<std::unique_ptr<LogicGate>> internalGates;
    std::vector<GatePinMap> pinMaps;

    void buildInternalGates();

public:
    ICChip(ICType icType, int startCol);

    // Moveable, non-copyable (owns unique_ptrs)
    ICChip(ICChip&& other) noexcept;
    ICChip& operator=(ICChip&& other) noexcept;
    ICChip(const ICChip&) = delete;
    ICChip& operator=(const ICChip&) = delete;

    ICType getType() const;
    const std::string& getLabel() const;
    int getFirstCol() const;
    int getPinCount() const;
    bool isPowered() const;

    // Breadboard coordinates for 1-based pin (1..14)
    int pinColumn(int pin) const;
    bool pinIsTopHalf(int pin) const; // true = ROW_E (pins 8..14), false = ROW_F (pins 1..7)

    bool isOutputPin(int pin) const;
    std::string pinDescription(int pin) const;

    // Evaluate all internal gates. Outputs are only actively driven when hasVcc && hasGnd.
    void evaluate(bool hasVcc, bool hasGnd,
                  const bool pinInputHigh[15],
                  bool pinOutputHigh[15],
                  bool pinOutputDriven[15]);
};
