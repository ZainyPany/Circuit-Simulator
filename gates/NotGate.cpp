#include "gates/NotGate.hpp"

NotGate::NotGate(const std::string& label, int x, int y)
    : LogicGate("NOT", 1, label, x, y) {}

bool NotGate::computeOutput(const std::vector<bool>& inputs) const {
    if (inputs.empty()) return true;
    return !inputs[0];
}
