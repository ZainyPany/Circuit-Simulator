#include "io/LED.hpp"

LED::LED(Color ledColor, const std::string& label, int x, int y)
    : IONode("LED", label, x, y), color(ledColor), lit(false) {
    addPin(Pin::Direction::Input);
}

LED::Color LED::getColor() const {
    return color;
}

bool LED::isOn() const {
    return lit;
}

void LED::setLit(bool value) {
    lit = value;
}

Pin* LED::getInputPin() const {
    std::vector<Pin*> ins = getInputPins();
    if (ins.empty()) return nullptr;
    return ins[0];
}

void LED::evaluate() {
    Pin* in = getInputPin();
    if (in != nullptr) {
        lit = in->getState();
    } else {
        lit = false;
    }
}
