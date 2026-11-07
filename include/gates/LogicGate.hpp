#pragma once
#include <iostream>
#include "core/Component.hpp"

// TASK (Feature 1 - Core Simulation Engine)
//
// LogicGate: abstract intermediate class for all gates. Derives from Component.
//
// TODO:
//  - Constructor creating the right number of input pins + 1 output pin.
//  - Override evaluate(): gather input states, call computeOutput(), write output pin.
//  - Pure virtual: bool computeOutput(const std::vector<bool>& inputs) const
//    (each concrete gate implements its own truth table).

class LogicGate : public Component {
    // TODO: members + public interface described above
};
