#pragma once

class Component;

class Pin {
public:
    enum class Direction { Input, Output };

private:
    Direction direction;
    bool signalState;
    bool dirty;
    Component* owner;

public:
    explicit Pin(Direction dir, Component* ownerComponent = nullptr);

    bool getState() const;
    void setState(bool state);

    Direction getDirection() const;

    Component* getOwner() const;
    void setOwner(Component* ownerComponent);

    bool isDirty() const;
    void clearDirty();
};
