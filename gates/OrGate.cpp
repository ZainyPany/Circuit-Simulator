#include "gates/OrGate.hpp"

OrGate::OrGate(int inputCount, const std::string& label, int x, int y)
    : LogicGate("OR", inputCount, label, x, y) {}

bool OrGate::computeOutput(const std::vector<bool>& inputs) const {
    for (std::size_t i = 0; i < inputs.size(); ++i) {
        if (inputs[i]) return true;
    }
    return false;
}
