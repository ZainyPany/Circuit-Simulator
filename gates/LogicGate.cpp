#include "gates/LogicGate.hpp"

LogicGate::LogicGate(const std::string& type, int inputCount, const std::string& label, int x, int y)
    : Component(type, label, x, y) {
    for (int i = 0; i < inputCount; ++i) {
        addPin(Pin::Direction::Input);
    }
    addPin(Pin::Direction::Output);
}

LogicGate::~LogicGate() {}

void LogicGate::evaluate() {
    std::vector<Pin*> inPins = getInputPins();
    std::vector<bool> inputStates;
    inputStates.reserve(inPins.size());
    for (std::size_t i = 0; i < inPins.size(); ++i) {
        inputStates.push_back(inPins[i]->getState());
    }

    Pin* out = getOutputPin();
    if (out != nullptr) {
        out->setState(computeOutput(inputStates));
    }
}

Pin* LogicGate::getOutputPin() const {
    std::vector<Pin*> outs = getOutputPins();
    if (outs.empty()) return nullptr;
    return outs[0];
}
