#include "core/NetworkManager.hpp"
#include <algorithm>

NetworkManager::NetworkManager(int maxIter) : maxIterations(maxIter) {}

void NetworkManager::addComponent(Component* comp) {
    if (comp == nullptr) return;
    if (std::find(components.begin(), components.end(), comp) == components.end()) {
        components.push_back(comp);
    }
}

void NetworkManager::removeComponent(Component* comp) {
    if (comp == nullptr) return;

    components.erase(std::remove(components.begin(), components.end(), comp), components.end());

    // Remove wires attached to any pin owned by comp
    std::vector<Wire*> toRemove;
    for (std::size_t i = 0; i < wires.size(); ++i) {
        Wire* w = wires[i];
        if (w->getSource()->getOwner() == comp || w->getDestination()->getOwner() == comp) {
            toRemove.push_back(w);
        }
    }
    for (std::size_t i = 0; i < toRemove.size(); ++i) {
        removeConnection(toRemove[i]);
    }
}

void NetworkManager::addConnection(Wire* wire) {
    if (wire == nullptr || !wire->isValid()) return;
    wires.push_back(wire);
    adjacency[wire->getSource()].push_back(wire);
    addComponent(wire->getSource()->getOwner());
    addComponent(wire->getDestination()->getOwner());
}

void NetworkManager::removeConnection(Wire* wire) {
    if (wire == nullptr) return;
    wires.erase(std::remove(wires.begin(), wires.end(), wire), wires.end());

    Pin* src = wire->getSource();
    std::unordered_map<Pin*, std::vector<Wire*>>::iterator it = adjacency.find(src);
    if (it != adjacency.end()) {
        std::vector<Wire*>& list = it->second;
        list.erase(std::remove(list.begin(), list.end(), wire), list.end());
        if (list.empty()) {
            adjacency.erase(it);
        }
    }
}

bool NetworkManager::step() {
    for (int iter = 0; iter < maxIterations; ++iter) {
        bool anyChange = false;

        // Evaluate all components
        for (std::size_t i = 0; i < components.size(); ++i) {
            components[i]->evaluate();
        }

        // Propagate along all wires
        for (std::size_t i = 0; i < wires.size(); ++i) {
            Pin* dst = wires[i]->getDestination();
            bool before = dst->getState();
            wires[i]->propagate();
            if (dst->getState() != before) {
                anyChange = true;
            }
        }

        // Clear dirty flags
        for (std::size_t i = 0; i < components.size(); ++i) {
            for (std::size_t p = 0; p < components[i]->getPinCount(); ++p) {
                if (components[i]->getPin(p)->isDirty()) {
                    anyChange = true;
                    components[i]->getPin(p)->clearDirty();
                }
            }
        }

        if (!anyChange) {
            return true;
        }
    }
    return false;
}

void NetworkManager::reset() {
    for (std::size_t i = 0; i < components.size(); ++i) {
        for (std::size_t p = 0; p < components[i]->getPinCount(); ++p) {
            components[i]->getPin(p)->setState(false);
            components[i]->getPin(p)->clearDirty();
        }
    }
}
