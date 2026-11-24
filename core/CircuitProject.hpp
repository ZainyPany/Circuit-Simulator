#pragma once
#include <memory>
#include <string>
#include <vector>
#include "core/Component.hpp"
#include "core/NetworkManager.hpp"
#include "core/Wire.hpp"

class CircuitProject {
public:
    enum class SimulationState { Running, Paused };

private:
    std::string title;
    std::string creationDate;
    SimulationState simState;
    std::vector<std::unique_ptr<Component>> components;
    std::vector<std::unique_ptr<Wire>> wires;
    NetworkManager network;

public:
    explicit CircuitProject(const std::string& projectTitle = "Untitled Circuit",
                            const std::string& date = "2026-11-24");

    const std::string& getTitle() const;
    void setTitle(const std::string& newTitle);
    const std::string& getCreationDate() const;

    SimulationState getSimulationState() const;
    void start();
    void pause();
    void clear();

    Component* addComponent(std::unique_ptr<Component> comp);
    bool removeComponent(int componentId);
    Component* findComponent(int componentId) const;

    Wire* addWire(Pin* source, Pin* destination, const std::string& color = "Yellow");
    bool removeWire(Wire* wire);

    void update();

    const std::vector<std::unique_ptr<Component>>& getComponents() const;
    const std::vector<std::unique_ptr<Wire>>& getWires() const;
};
