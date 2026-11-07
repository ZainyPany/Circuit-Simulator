#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "core/Component.hpp"
#include "core/Wire.hpp"

// TASK (Feature 3 - Wiring & Signal Propagation)
//
// NetworkManager: holds the circuit graph (adjacency list: output pin -> input pins)
// and resolves logic across the whole network.
//
// TODO:
//  - Adjacency list tracking which output pins feed which input pins.
//  - addConnection(Wire*) / removeConnection(Wire*) / removeComponent(Component*).
//  - step(): one simulation tick - evaluate components and propagate wires.
//  - Traverse the graph (BFS/DFS or topological order) to cascade updates.
//  - Detect combinational loops / unstable feedback and stop after a max iteration count.
//  - reset(): set all pins to Low.
//  - Decide how Clock components trigger a tick (called from the main loop).

class NetworkManager {
    // TODO: members + public interface described above
};
