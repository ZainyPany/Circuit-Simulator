#include "core/Component.hpp"

int Component::nextId = 1;

Component::Component(const std::string& type, const std::string& compLabel, int x, int y)
    : id(nextId++), label(compLabel), deviceType(type), gridX(x), gridY(y) {}

Component::~Component() {}

Component::Component(Component&& other) noexcept
    : id(other.id),
      label(std::move(other.label)),
      deviceType(std::move(other.deviceType)),
      gridX(other.gridX),
      gridY(other.gridY),
      pins(std::move(other.pins)) {
    for (std::size_t i = 0; i < pins.size(); ++i) {
        if (pins[i]) {
            pins[i]->setOwner(this);
        }
    }
}

Component& Component::operator=(Component&& other) noexcept {
    if (this != &other) {
        id = other.id;
        label = std::move(other.label);
        deviceType = std::move(other.deviceType);
        gridX = other.gridX;
        gridY = other.gridY;
        pins = std::move(other.pins);
        for (std::size_t i = 0; i < pins.size(); ++i) {
            if (pins[i]) {
                pins[i]->setOwner(this);
            }
        }
    }
    return *this;
}

Pin* Component::addPin(Pin::Direction dir) {
    std::unique_ptr<Pin> p(new Pin(dir, this));
    Pin* raw = p.get();
    pins.push_back(std::move(p));
    return raw;
}

int Component::getId() const {
    return id;
}

const std::string& Component::getLabel() const {
    return label;
}

void Component::setLabel(const std::string& newLabel) {
    label = newLabel;
}

const std::string& Component::getDeviceType() const {
    return deviceType;
}

int Component::getGridX() const {
    return gridX;
}

int Component::getGridY() const {
    return gridY;
}

void Component::setGridPosition(int x, int y) {
    gridX = x;
    gridY = y;
}

std::vector<Pin*> Component::getInputPins() const {
    std::vector<Pin*> result;
    for (std::size_t i = 0; i < pins.size(); ++i) {
        if (pins[i]->getDirection() == Pin::Direction::Input) {
            result.push_back(pins[i].get());
        }
    }
    return result;
}

std::vector<Pin*> Component::getOutputPins() const {
    std::vector<Pin*> result;
    for (std::size_t i = 0; i < pins.size(); ++i) {
        if (pins[i]->getDirection() == Pin::Direction::Output) {
            result.push_back(pins[i].get());
        }
    }
    return result;
}

Pin* Component::getPin(std::size_t index) const {
    if (index >= pins.size()) return nullptr;
    return pins[index].get();
}

std::size_t Component::getPinCount() const {
    return pins.size();
}
