#pragma once
#include <string>
#include "core/Component.hpp"

class IONode : public Component {
public:
    IONode(const std::string& type, const std::string& label = "", int x = 0, int y = 0);
    virtual ~IONode();

    IONode(IONode&&) noexcept = default;
    IONode& operator=(IONode&&) noexcept = default;

    bool isInputDevice() const;
    bool isOutputDevice() const;
};
