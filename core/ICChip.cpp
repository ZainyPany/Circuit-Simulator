#include "core/ICChip.hpp"
#include "gates/AndGate.hpp"
#include "gates/NotGate.hpp"
#include "gates/OrGate.hpp"
#include "gates/XorGate.hpp"

ICChip::ICChip(ICType icType, int startCol)
    : type(icType), firstCol(startCol), powered(false) {
    switch (type) {
        case ICType::IC7408_AND: label = "7408 AND"; break;
        case ICType::IC7404_NOT: label = "7404 NOT"; break;
        case ICType::IC7432_OR:  label = "7432 OR";  break;
        case ICType::IC7486_XOR: label = "7486 XOR"; break;
    }
    buildInternalGates();
}

ICChip::ICChip(ICChip&& other) noexcept
    : type(other.type),
      label(std::move(other.label)),
      firstCol(other.firstCol),
      powered(other.powered),
      internalGates(std::move(other.internalGates)),
      pinMaps(std::move(other.pinMaps)) {}

ICChip& ICChip::operator=(ICChip&& other) noexcept {
    if (this != &other) {
        type = other.type;
        label = std::move(other.label);
        firstCol = other.firstCol;
        powered = other.powered;
        internalGates = std::move(other.internalGates);
        pinMaps = std::move(other.pinMaps);
    }
    return *this;
}

void ICChip::buildInternalGates() {
    internalGates.clear();
    pinMaps.clear();

    if (type == ICType::IC7404_NOT) {
        // Hex inverter: 6 NOT gates
        const GatePinMap hexMap[6] = {
            {1, 0, 2}, {3, 0, 4}, {5, 0, 6},
            {9, 0, 8}, {11, 0, 10}, {13, 0, 12}
        };
        for (int i = 0; i < 6; ++i) {
            pinMaps.push_back(hexMap[i]);
            internalGates.push_back(std::unique_ptr<LogicGate>(new NotGate()));
        }
    } else {
        // Quad 2-input gates: 7408 (AND), 7432 (OR), 7486 (XOR)
        const GatePinMap quadMap[4] = {
            {1, 2, 3}, {4, 5, 6}, {9, 10, 8}, {12, 13, 11}
        };
        for (int i = 0; i < 4; ++i) {
            pinMaps.push_back(quadMap[i]);
            if (type == ICType::IC7408_AND) {
                internalGates.push_back(std::unique_ptr<LogicGate>(new AndGate()));
            } else if (type == ICType::IC7432_OR) {
                internalGates.push_back(std::unique_ptr<LogicGate>(new OrGate()));
            } else {
                internalGates.push_back(std::unique_ptr<LogicGate>(new XorGate()));
            }
        }
    }
}

ICType ICChip::getType() const {
    return type;
}

const std::string& ICChip::getLabel() const {
    return label;
}

int ICChip::getFirstCol() const {
    return firstCol;
}

int ICChip::getPinCount() const {
    return 14;
}

bool ICChip::isPowered() const {
    return powered;
}

int ICChip::pinColumn(int pin) const {
    if (pin <= 7) return firstCol + (pin - 1);
    return firstCol + (14 - pin);
}

bool ICChip::pinIsTopHalf(int pin) const {
    return pin > 7;
}

bool ICChip::isOutputPin(int pin) const {
    for (std::size_t i = 0; i < pinMaps.size(); ++i) {
        if (pinMaps[i].out == pin) return true;
    }
    return false;
}

std::string ICChip::pinDescription(int pin) const {
    if (pin == 14) return label + " Pin 14 (VCC +5V)";
    if (pin == 7)  return label + " Pin 7 (GND)";

    for (std::size_t i = 0; i < pinMaps.size(); ++i) {
        std::string gateNum = std::to_string(i + 1);
        if (pinMaps[i].in1 == pin) {
            return label + " Pin " + std::to_string(pin) + " (Gate " + gateNum + " In A)";
        }
        if (pinMaps[i].in2 == pin && pinMaps[i].in2 != 0) {
            return label + " Pin " + std::to_string(pin) + " (Gate " + gateNum + " In B)";
        }
        if (pinMaps[i].out == pin) {
            return label + " Pin " + std::to_string(pin) + " (Gate " + gateNum + " Out Y)";
        }
    }
    return label + " Pin " + std::to_string(pin);
}

void ICChip::evaluate(bool hasVcc, bool hasGnd,
                      const bool pinInputHigh[15],
                      bool pinOutputHigh[15],
                      bool pinOutputDriven[15]) {
    for (int p = 0; p < 15; ++p) {
        pinOutputHigh[p] = false;
        pinOutputDriven[p] = false;
    }

    powered = (hasVcc && hasGnd);
    if (!powered) {
        return;
    }

    for (std::size_t i = 0; i < internalGates.size(); ++i) {
        const GatePinMap& m = pinMaps[i];
        LogicGate* gate = internalGates[i].get();
        std::vector<Pin*> ins = gate->getInputPins();

        if (ins.size() >= 1) {
            ins[0]->setState(pinInputHigh[m.in1]);
        }
        if (ins.size() >= 2 && m.in2 != 0) {
            ins[1]->setState(pinInputHigh[m.in2]);
        }

        gate->evaluate();

        Pin* outPin = gate->getOutputPin();
        bool outVal = (outPin != nullptr) ? outPin->getState() : false;
        pinOutputHigh[m.out] = outVal;
        pinOutputDriven[m.out] = true;
    }
}
