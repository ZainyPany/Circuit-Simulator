#include "io/Switch.hpp"

Switch::Switch(const std::string& label, bool initialState, int x, int y)
    : IONode("SWITCH", label, x, y), state(initialState) {
    Pin* out = addPin(Pin::Direction::Output);
    out->setState(state);
}

bool Switch::isOn() const {
    return state;
}

void Switch::setState(bool newState) {
    state = newState;
    evaluate();
}

void Switch::toggle() {
    state = !state;
    evaluate();
}

Pin* Switch::getOutputPin() const {
    std::vector<Pin*> outs = getOutputPins();
    if (outs.empty()) return nullptr;
    return outs[0];
}

void Switch::evaluate() {
    Pin* out = getOutputPin();
    if (out != nullptr) {
        out->setState(state);
    }
}
