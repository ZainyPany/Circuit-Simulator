#pragma once
#include <unordered_map>
#include <vector>
#include "core/Component.hpp"
#include "core/Wire.hpp"

class NetworkManager {
private:
    std::vector<Component*> components;
    std::vector<Wire*> wires;
    std::unordered_map<Pin*, std::vector<Wire*>> adjacency;
    int maxIterations;

public:
    explicit NetworkManager(int maxIter = 64);

    void addComponent(Component* comp);
    void removeComponent(Component* comp);

    void addConnection(Wire* wire);
    void removeConnection(Wire* wire);

    // Evaluates all components and propagates signals until stable or maxIterations reached.
    // Returns true if settled, false if combinational loop hit maxIterations.
    bool step();
    void reset();
};
