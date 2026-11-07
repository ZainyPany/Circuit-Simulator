#pragma once
#include <iostream>
#include <vector>
#include "core/Pin.hpp"

// TASK (Feature 3 - Wiring & Signal Propagation)
//
// Wire: connects ONE output Pin to ONE input Pin and transfers state between them.
//
// TODO:
//  - Data: pointer to source Pin (output), pointer to destination Pin (input),
//          color, routingPath (list of grid points), currentSignal.
//  - Constructor validating the connection (output -> input only, no output -> output).
//  - propagate(): copy source state to destination pin.
//  - Getters for both pins, color (changes with High/Low), routing path.
//  - Decide: should the Wire own the Pins or only point to them? (Pins belong to Components.)

class Wire {
    // TODO: members + public interface described above
};
