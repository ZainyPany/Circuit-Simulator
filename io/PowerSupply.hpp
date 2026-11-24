#pragma once
#include <string>
#include "io/IONode.hpp"

class PowerSupply : public IONode {
private:
    float nominalVoltage;
    bool enabled;

public:
    explicit PowerSupply(float voltage = 5.0f, bool initiallyOn = true,
                         const std::string& label = "5V DC", int x = 0, int y = 0);

    bool isEnabled() const;
    void setEnabled(bool on);
    void toggle();

    float getNominalVoltage() const;
    float getOutputVoltage() const;

    Pin* getVccPin() const;
    void evaluate() override;
};
