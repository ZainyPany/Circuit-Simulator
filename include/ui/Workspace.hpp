#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "core/CircuitProject.hpp"

// TASK (Feature 2 - Canvas Rendering & UI Interactions)
//
// Workspace: the grid canvas. Draws the circuit and handles mouse input.
//
// TODO:
//  - Draw the grid, all components (shapes/labels per type), pins, and wires
//    (wire color depends on High/Low).
//  - Drag-and-drop: pick a component from the palette, place it snapped to the grid.
//  - Move placed components by dragging; delete with a key / right-click.
//  - Wire creation: click an output pin, drag, release on an input pin.
//  - Click a Switch to toggle it.
//  - handleEvent(const sf::Event&), update(float dt), draw(sf::RenderTarget&).
//  - Convert between pixel coordinates and grid coordinates.
//  - Hooks: toFile()/fromFile() that gather component states and connections
//    and pass them to FileIO.

class Workspace {
    // TODO: members + public interface described above
};
