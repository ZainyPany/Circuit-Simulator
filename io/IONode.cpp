#include "io/IONode.hpp"

IONode::IONode(const std::string& type, const std::string& label, int x, int y)
    : Component(type, label, x, y) {}

IONode::~IONode() {}

bool IONode::isInputDevice() const {
    return !getOutputPins().empty() && getInputPins().empty();
}

bool IONode::isOutputDevice() const {
    return !getInputPins().empty() && getOutputPins().empty();
}
