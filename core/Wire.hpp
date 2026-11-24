#pragma once
#include <string>
#include <utility>
#include <vector>
#include "core/Pin.hpp"

class Wire {
private:
    Pin* sourcePin;
    Pin* destinationPin;
    std::string colorName;
    std::vector<std::pair<int, int>> routingPath;
    bool currentSignal;

public:
    Wire(Pin* source, Pin* destination, const std::string& color = "Yellow");

    bool isValid() const;
    void propagate();

    Pin* getSource() const;
    Pin* getDestination() const;

    bool getSignal() const;
    const std::string& getColor() const;
    void setColor(const std::string& color);

    const std::vector<std::pair<int, int>>& getRoutingPath() const;
    void setRoutingPath(const std::vector<std::pair<int, int>>& path);
};
