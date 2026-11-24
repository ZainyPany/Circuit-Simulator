#include "core/Pin.hpp"

Pin::Pin(Direction dir, Component* ownerComponent)
    : direction(dir), signalState(false), dirty(false), owner(ownerComponent) {}

bool Pin::getState() const {
    return signalState;
}

void Pin::setState(bool state) {
    if (signalState != state) {
        signalState = state;
        dirty = true;
    }
}

Pin::Direction Pin::getDirection() const {
    return direction;
}

Component* Pin::getOwner() const {
    return owner;
}

void Pin::setOwner(Component* ownerComponent) {
    owner = ownerComponent;
}

bool Pin::isDirty() const {
    return dirty;
}

void Pin::clearDirty() {
    dirty = false;
}
