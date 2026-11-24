#include "gates/XorGate.hpp"

XorGate::XorGate(int inputCount, const std::string& label, int x, int y)
    : LogicGate("XOR", inputCount, label, x, y) {}

bool XorGate::computeOutput(const std::vector<bool>& inputs) const {
    bool result = false;
    for (std::size_t i = 0; i < inputs.size(); ++i) {
        result = (result != inputs[i]);
    }
    return result;
}
