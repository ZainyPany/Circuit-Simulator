#include "io/Clock.hpp"

Clock::Clock(float periodSeconds, const std::string& label, int x, int y)
    : IONode("CLOCK", label, x, y), period(periodSeconds > 0.05f ? periodSeconds : 0.05f),
      elapsed(0.0f), state(false) {
    addPin(Pin::Direction::Output);
}

float Clock::getPeriod() const {
    return period;
}

void Clock::setPeriod(float periodSeconds) {
    if (periodSeconds >= 0.05f) {
        period = periodSeconds;
    }
}

bool Clock::isHigh() const {
    return state;
}

void Clock::update(float dt) {
    elapsed += dt;
    float halfPeriod = period * 0.5f;
    if (elapsed >= halfPeriod) {
        elapsed -= halfPeriod;
        state = !state;
        evaluate();
    }
}

Pin* Clock::getOutputPin() const {
    std::vector<Pin*> outs = getOutputPins();
    if (outs.empty()) return nullptr;
    return outs[0];
}

void Clock::evaluate() {
    Pin* out = getOutputPin();
    if (out != nullptr) {
        out->setState(state);
    }
}
