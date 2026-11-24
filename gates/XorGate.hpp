#pragma once
#include "gates/LogicGate.hpp"

class XorGate : public LogicGate {
public:
    explicit XorGate(int inputCount = 2, const std::string& label = "XOR", int x = 0, int y = 0);
    bool computeOutput(const std::vector<bool>& inputs) const override;
};
