#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "core/Pin.hpp"

// TASK (Feature 1 - Core Simulation Engine)
//
// Component: abstract base class for everything placed on the canvas
// (LogicGate and IONode derive from it).
//
// TODO:
//  - Data: unique ID, label, deviceType, grid position (x, y), vector of owned Pins.
//  - Static ID counter so every Component gets a unique ID automatically.
//  - Getters/setters for position, label, ID, deviceType.
//  - addPin(), getInputPins(), getOutputPins(), getPin(index).
//  - Pure virtual: void evaluate()  -> reads input pins, writes output pins.
//  - Virtual destructor.
//  - Decide how a Component reports its type name for saving to file (e.g. "AND", "SWITCH").

class Component {
    // TODO: members + public interface described above
};
