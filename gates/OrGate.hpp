#pragma once
#include "gates/LogicGate.hpp"

class OrGate : public LogicGate {
public:
    explicit OrGate(int inputCount = 2, const std::string& label = "OR", int x = 0, int y = 0);
    bool computeOutput(const std::vector<bool>& inputs) const override;
};
