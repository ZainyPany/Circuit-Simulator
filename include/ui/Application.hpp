#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

// TASK (Glue / main loop)
//
// Application: owns the sf::RenderWindow and switches between screens
// (MainMenu -> Workspace + ComponentPalette + ControlRibbon -> SaveLoadDialog).
//
// TODO:
//  - Screen state enum (Menu, Editor, Dialog).
//  - run(): the main loop - poll events, update(dt), draw().
//  - Connect ControlRibbon button clicks to CircuitProject (start/pause/clear)
//    and to SaveLoadDialog/FileIO.
//  - Connect ComponentPalette selection to Workspace.

class Application {
    // TODO: members + public interface described above
};
