#include "io/PushButton.hpp"

PushButton::PushButton(const std::string& label, int x, int y)
    : Switch(label, false, x, y), momentaryPressed(false), latched(false) {}

void PushButton::syncState() {
    setState(momentaryPressed || latched);
}

void PushButton::press() {
    momentaryPressed = true;
    syncState();
}

void PushButton::release() {
    momentaryPressed = false;
    syncState();
}

void PushButton::toggleLatch() {
    latched = !latched;
    syncState();
}

void PushButton::setLatched(bool value) {
    latched = value;
    syncState();
}

bool PushButton::isPressed() const {
    return isOn();
}

bool PushButton::isLatched() const {
    return latched;
}
