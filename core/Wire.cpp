#include "core/Wire.hpp"
#include <stdexcept>

Wire::Wire(Pin* source, Pin* destination, const std::string& color)
    : sourcePin(source), destinationPin(destination), colorName(color), currentSignal(false) {
    if (sourcePin == nullptr || destinationPin == nullptr) {
        throw std::invalid_argument("Wire endpoints cannot be null.");
    }
    if (sourcePin->getDirection() != Pin::Direction::Output ||
        destinationPin->getDirection() != Pin::Direction::Input) {
        throw std::invalid_argument("Wire must connect an Output pin to an Input pin.");
    }
}

bool Wire::isValid() const {
    return sourcePin != nullptr && destinationPin != nullptr &&
           sourcePin->getDirection() == Pin::Direction::Output &&
           destinationPin->getDirection() == Pin::Direction::Input;
}

void Wire::propagate() {
    if (!isValid()) return;
    currentSignal = sourcePin->getState();
    destinationPin->setState(currentSignal);
}

Pin* Wire::getSource() const {
    return sourcePin;
}

Pin* Wire::getDestination() const {
    return destinationPin;
}

bool Wire::getSignal() const {
    return currentSignal;
}

const std::string& Wire::getColor() const {
    return colorName;
}

void Wire::setColor(const std::string& color) {
    colorName = color;
}

const std::vector<std::pair<int, int>>& Wire::getRoutingPath() const {
    return routingPath;
}

void Wire::setRoutingPath(const std::vector<std::pair<int, int>>& path) {
    routingPath = path;
}
