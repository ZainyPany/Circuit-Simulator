#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "core/Component.hpp"
#include "core/Wire.hpp"
#include "core/NetworkManager.hpp"

// TASK (Domain model)
//
// CircuitProject: the data model of one circuit. Owns all Components and Wires.
// (Matches the "CircuitProject" box in the domain class diagram.)
//
// TODO:
//  - Data: title, creationDate, simulationState (Running/Paused), components, wires, NetworkManager.
//  - addComponent(unique_ptr<Component>), removeComponent(id), findComponent(id).
//  - addWire(...), removeWire(...).
//  - start() / pause() / clear() the simulation.
//  - update(): called every frame, advances the simulation if Running.
//  - Provide read access for Workspace (rendering) and FileIO (saving).

class CircuitProject {
    // TODO: members + public interface described above
};
