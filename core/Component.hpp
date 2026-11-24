#pragma once
#include <memory>
#include <string>
#include <vector>
#include "core/Pin.hpp"

class Component {
private:
    static int nextId;
    int id;
    std::string label;
    std::string deviceType;
    int gridX;
    int gridY;

protected:
    std::vector<std::unique_ptr<Pin>> pins;

    Pin* addPin(Pin::Direction dir);

public:
    Component(const std::string& type, const std::string& compLabel = "", int x = 0, int y = 0);
    virtual ~Component();

    Component(Component&& other) noexcept;
    Component& operator=(Component&& other) noexcept;
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;

    int getId() const;
    const std::string& getLabel() const;
    void setLabel(const std::string& newLabel);

    const std::string& getDeviceType() const;

    int getGridX() const;
    int getGridY() const;
    void setGridPosition(int x, int y);

    std::vector<Pin*> getInputPins() const;
    std::vector<Pin*> getOutputPins() const;
    Pin* getPin(std::size_t index) const;
    std::size_t getPinCount() const;

    virtual void evaluate() = 0;
};
