#pragma once
#include "gates/LogicGate.hpp"

class NotGate : public LogicGate {
public:
    explicit NotGate(const std::string& label = "NOT", int x = 0, int y = 0);
    bool computeOutput(const std::vector<bool>& inputs) const override;
};
