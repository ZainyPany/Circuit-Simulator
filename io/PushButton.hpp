#pragma once
#include <string>
#include "io/Switch.hpp"

// Tactile PushButton: bridges its input rail/node to its output node when closed.
// Supports both momentary press/release and click-to-latch toggling for easy mouse demos.
class PushButton : public Switch {
private:
    bool momentaryPressed;
    bool latched;

    void syncState();

public:
    explicit PushButton(const std::string& label = "BTN", int x = 0, int y = 0);

    void press();
    void release();
    void toggleLatch();
    void setLatched(bool value);

    bool isPressed() const;
    bool isLatched() const;
};
