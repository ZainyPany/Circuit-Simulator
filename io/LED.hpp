#pragma once
#include <string>
#include "io/IONode.hpp"

class LED : public IONode {
public:
    enum class Color { Red, Green, Blue, Yellow };

private:
    Color color;
    bool lit;

public:
    explicit LED(Color ledColor = Color::Red, const std::string& label = "LED", int x = 0, int y = 0);

    Color getColor() const;
    bool isOn() const;
    void setLit(bool value);

    Pin* getInputPin() const;
    void evaluate() override;
};
