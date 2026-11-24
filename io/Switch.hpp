#pragma once
#include <string>
#include "io/IONode.hpp"

class Switch : public IONode {
protected:
    bool state;

public:
    explicit Switch(const std::string& label = "SW", bool initialState = false, int x = 0, int y = 0);

    bool isOn() const;
    void setState(bool newState);
    void toggle();

    Pin* getOutputPin() const;
    void evaluate() override;
};
