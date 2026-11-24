#pragma once
#include <string>
#include "io/IONode.hpp"

class Clock : public IONode {
private:
    float period;
    float elapsed;
    bool state;

public:
    explicit Clock(float periodSeconds = 1.0f, const std::string& label = "CLK", int x = 0, int y = 0);

    float getPeriod() const;
    void setPeriod(float periodSeconds);
    bool isHigh() const;

    void update(float dt);
    Pin* getOutputPin() const;
    void evaluate() override;
};
