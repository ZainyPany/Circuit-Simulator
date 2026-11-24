#include "core/CircuitProject.hpp"

CircuitProject::CircuitProject(const std::string& projectTitle, const std::string& date)
    : title(projectTitle), creationDate(date), simState(SimulationState::Running) {}

const std::string& CircuitProject::getTitle() const {
    return title;
}

void CircuitProject::setTitle(const std::string& newTitle) {
    title = newTitle;
}

const std::string& CircuitProject::getCreationDate() const {
    return creationDate;
}

CircuitProject::SimulationState CircuitProject::getSimulationState() const {
    return simState;
}

void CircuitProject::start() {
    simState = SimulationState::Running;
}

void CircuitProject::pause() {
    simState = SimulationState::Paused;
}

void CircuitProject::clear() {
    wires.clear();
    components.clear();
    network = NetworkManager();
}

Component* CircuitProject::addComponent(std::unique_ptr<Component> comp) {
    if (!comp) return nullptr;
    Component* raw = comp.get();
    network.addComponent(raw);
    components.push_back(std::move(comp));
    return raw;
}

bool CircuitProject::removeComponent(int componentId) {
    for (std::size_t i = 0; i < components.size(); ++i) {
        if (components[i]->getId() == componentId) {
            Component* target = components[i].get();
            network.removeComponent(target);

            // Erase any wires owned by CircuitProject connected to this component
            std::vector<std::unique_ptr<Wire>> remainingWires;
            for (std::size_t w = 0; w < wires.size(); ++w) {
                if (wires[w]->getSource()->getOwner() != target &&
                    wires[w]->getDestination()->getOwner() != target) {
                    remainingWires.push_back(std::move(wires[w]));
                }
            }
            wires = std::move(remainingWires);
            components.erase(components.begin() + static_cast<std::ptrdiff_t>(i));
            return true;
        }
    }
    return false;
}

Component* CircuitProject::findComponent(int componentId) const {
    for (std::size_t i = 0; i < components.size(); ++i) {
        if (components[i]->getId() == componentId) {
            return components[i].get();
        }
    }
    return nullptr;
}

Wire* CircuitProject::addWire(Pin* source, Pin* destination, const std::string& color) {
    std::unique_ptr<Wire> w(new Wire(source, destination, color));
    Wire* raw = w.get();
    network.addConnection(raw);
    wires.push_back(std::move(w));
    return raw;
}

bool CircuitProject::removeWire(Wire* wire) {
    for (std::size_t i = 0; i < wires.size(); ++i) {
        if (wires[i].get() == wire) {
            network.removeConnection(wire);
            wires.erase(wires.begin() + static_cast<std::ptrdiff_t>(i));
            return true;
        }
    }
    return false;
}

void CircuitProject::update() {
    if (simState == SimulationState::Running) {
        network.step();
    }
}

const std::vector<std::unique_ptr<Component>>& CircuitProject::getComponents() const {
    return components;
}

const std::vector<std::unique_ptr<Wire>>& CircuitProject::getWires() const {
    return wires;
}
