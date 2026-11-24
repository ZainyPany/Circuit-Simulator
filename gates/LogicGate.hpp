#pragma once
#include <string>
#include <vector>
#include "core/Component.hpp"

class LogicGate : public Component {
public:
    LogicGate(const std::string& type, int inputCount, const std::string& label = "", int x = 0, int y = 0);
    virtual ~LogicGate();

    LogicGate(LogicGate&&) noexcept = default;
    LogicGate& operator=(LogicGate&&) noexcept = default;

    void evaluate() override;
    virtual bool computeOutput(const std::vector<bool>& inputs) const = 0;

    Pin* getOutputPin() const;
};
