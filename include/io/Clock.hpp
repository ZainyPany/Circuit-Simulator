#pragma once
#include <iostream>
#include "io/IONode.hpp"

// TASK (Feature 1)
//
// Clock: input that toggles automatically at a fixed interval. One output pin.
//
// TODO:
//  - Data: period / frequency, time accumulator.
//  - update(float dt): accumulate time and toggle output when the period passes.
//  - evaluate(): push the current state to the output pin.
//  - Optional: adjustable frequency from the UI.

class Clock : public IONode {
    // TODO: members + public interface described above
};
