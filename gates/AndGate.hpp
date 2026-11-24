#pragma once
#include "gates/LogicGate.hpp"

class AndGate : public LogicGate {
public:
    explicit AndGate(int inputCount = 2, const std::string& label = "AND", int x = 0, int y = 0);
    bool computeOutput(const std::vector<bool>& inputs) const override;
};
