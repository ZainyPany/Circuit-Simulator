#include "gates/AndGate.hpp"

AndGate::AndGate(int inputCount, const std::string& label, int x, int y)
    : LogicGate("AND", inputCount, label, x, y) {}

bool AndGate::computeOutput(const std::vector<bool>& inputs) const {
    if (inputs.empty()) return false;
    for (std::size_t i = 0; i < inputs.size(); ++i) {
        if (!inputs[i]) return false;
    }
    return true;
}
