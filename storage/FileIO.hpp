#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "core/CircuitProject.hpp"

// TASK (Feature 4 - Circuit Serialization)
//
// FileIO: reads/writes circuit files (.json) to disk.
//
// TODO:
//  - static bool save(const CircuitProject&, const std::string& path)
//  - static bool load(CircuitProject&, const std::string& path)
//  - JSON layout: list of components (id, type, label, x, y, state) + list of wires
//    (source component id + pin index, destination component id + pin index).
//  - Factory: create the right Component subclass from its saved type string.
//  - Handle errors: missing file, corrupted JSON, unknown component type.
//  - listProjects(directory): returns file names for the Save/Load dialog.
//  - Recent-files list for the startup menu (name + last modified).
//  - Decide the JSON library (e.g. nlohmann/json) or write a small parser.

class FileIO {
    // TODO: static functions described above
};
