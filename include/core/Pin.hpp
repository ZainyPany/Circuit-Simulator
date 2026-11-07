#pragma once
#include <iostream>

// TASK (Feature 1 - Core Simulation Engine)
//
// Pin: the connection point on a Component. Holds ONE binary state (High/Low).
//
// TODO:
//  - enum class Direction { Input, Output }
//  - Store: direction, signalState (bool), pointer to the owning Component.
//  - Constructor taking a Direction (default state = Low).
//  - getState() / setState(bool)
//  - getDirection()
//  - getOwner() / setOwner(Component*)  -> forward-declare Component to avoid a circular include.
//  - A way for the NetworkManager to know when a Pin's state changed (e.g. a "dirty" flag).

class Component;

class Pin {
    // TODO: members + public interface described above
};
