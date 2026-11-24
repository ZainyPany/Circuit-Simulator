#include "io/PowerSupply.hpp"

PowerSupply::PowerSupply(float voltage, bool initiallyOn, const std::string& label, int x, int y)
    : IONode("POWER_5V", label, x, y), nominalVoltage(voltage), enabled(initiallyOn) {
    Pin* vcc = addPin(Pin::Direction::Output);
    vcc->setState(enabled);
}

bool PowerSupply::isEnabled() const {
    return enabled;
}

void PowerSupply::setEnabled(bool on) {
    enabled = on;
    evaluate();
}

void PowerSupply::toggle() {
    enabled = !enabled;
    evaluate();
}

float PowerSupply::getNominalVoltage() const {
    return nominalVoltage;
}

float PowerSupply::getOutputVoltage() const {
    return enabled ? nominalVoltage : 0.0f;
}

Pin* PowerSupply::getVccPin() const {
    std::vector<Pin*> outs = getOutputPins();
    if (outs.empty()) return nullptr;
    return outs[0];
}

void PowerSupply::evaluate() {
    Pin* vcc = getVccPin();
    if (vcc != nullptr) {
        vcc->setState(enabled);
    }
}
