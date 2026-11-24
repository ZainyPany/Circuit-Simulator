# GEMINI.md — Project Progress & Session Memory

## Project Identity
- **Name:** Digital Logic Circuit Simulator
- **Course:** CS/CE 224/272 Object-Oriented Programming and Design Methodologies (Habib University, Fall 2026)
- **Team:** Muhammad Khan (`@mk11071`, 11071), Abbas Huzaifa Dar (`@achdiop`, 11013), Syed Zain Imran (`@ZainyPany`, 11682)
- **Proposal Reference:** `docs/Project_Proposal.pdf`
- **SFML Setup Reference:** `docs/sfml-guide/ucrt_sfml_setup with WSL-1.pdf`

## Technical Constraints & Conventions
- **Language Standard:** Strictly **C++17** (`-std=c++17`). Do **not** use C++20/C++23 features (`std::ranges`, `std::format`, `std::expected`, `std::span`, concepts, etc.).
- **Graphics Library:** **SFML 3.0** (`sf::VideoMode({w, h})`, `font.openFromFile(...)`, `window.pollEvent()` returning `std::optional<sf::Event>`, `event->is<sf::Event::Closed>()`, `event->getIf<...>()`, scoped enums `sf::Mouse::Button::Left`, `sf::Keyboard::Key::...`).
- **Compiler Environment (Windows):** MSYS2 UCRT64 (`C:\msys64\ucrt64\bin\g++.exe`) with `mingw-w64-ucrt-x86_64-sfml`.
- **Hardware Style:** Keep `hardware/` views consistent with `hardware/Theme.hpp` and `hardware/Draw.hpp` (`drawRect`, `drawCircle`, `drawText`, `drawLine`). Keep `hardware/preview.cpp` and `led_preview.cpp` compatible.
- **Breadboard Rail Gaps:** In `BreadboardView`, power rail holes are grouped in 5s (`RAIL_GROUP = 6`), so columns where `col % 6 == 5` (5, 11, 17, 23, 29, 35) have no rail hole drawn. Always avoid placing rail wire endpoints on those columns.
- **Documentation:** Keep `README.md` up to date whenever features, build commands, or controls change.

## Current Progress (Interim Demo — Complete)

### Implemented
- **Core Simulation Engine (`core/`):**
  - `Pin.{hpp,cpp}` — binary state, `Direction::Input/Output`, dirty flag, owner pointer.
  - `Component.{hpp,cpp}` — abstract base class with auto-incrementing ID, label, type, grid coords, owned `Pin`s, move semantics updating `Pin::owner`.
  - `Wire.{hpp,cpp}` — connects Output `Pin*` to Input `Pin*`, validates direction, propagates state.
  - `NetworkManager.{hpp,cpp}` — adjacency list, iterative signal propagation with max-iteration loop guard.
  - `CircuitProject.{hpp,cpp}` — owns components/wires, Running/Paused simulation state.
- **Logic Gates (`gates/`):**
  - `LogicGate.{hpp,cpp}`, `AndGate.{hpp,cpp}`, `OrGate.{hpp,cpp}`, `NotGate.{hpp,cpp}`, `XorGate.{hpp,cpp}`.
- **I/O Devices (`io/`):**
  - `IONode.{hpp,cpp}`, `Switch.{hpp,cpp}`, `PushButton.{hpp,cpp}` (latch + momentary), `LED.{hpp,cpp}` (Red/Green/Blue/Yellow), `Clock.{hpp,cpp}`, `PowerSupply.{hpp,cpp}` (5V DC ON/OFF).
- **Breadboard & 4 TTL ICs (`core/`):**
  - `ICChip.{hpp,cpp}` — 14-pin DIP models for `7408` (AND), `7404` (NOT), `7432` (OR), `7486` (XOR), composed of `LogicGate` instances, enforcing Pin 14 (VCC) + Pin 7 (GND) power.
  - `BreadboardCircuit.{hpp,cpp}` — 84-node Union-Find breadboard electrical solver, 5V supply, 4 ICs, 3 push buttons (`BTN A/B/C`), 3 LEDs (Red/Green/Blue), interactive jumper wiring, hover node probe, and 3 presets (`BasicGates`, `InvertedGates`, `Sandbox`).
- **Hardware SFML 3.0 Views (`hardware/`):**
  - Existing: `Theme.hpp`, `Draw.{hpp,cpp}`, `BreadboardView.{hpp,cpp}`, `ICView.{hpp,cpp}`, `LEDView.{hpp,cpp}`, `WireView.{hpp,cpp}`.
  - Added: `PowerSupplyView.{hpp,cpp}`, `PushButtonView.{hpp,cpp}`.
- **Application UI (`ui/` & `main.cpp`):**
  - `ui/Application.{hpp,cpp}` and `main.cpp` — full interactive interim demo window with toolbar presets, wire color palette, click-to-wire breadboard holes, undo/reset, clickable 5V supply & push buttons, and live hover voltage/pin probe.
- **Tests & Docs:**
  - `tests/sim_test.cpp` — headless C++17 unit tests (passing with `-Wall -Wextra -Wpedantic`).
  - `README.md` — full overview, build commands, and controls.

### Remaining (Future Milestones from Proposal)
- `ui/MainMenu.{hpp,cpp}` — startup menu with recent projects list.
- `ui/ComponentPalette.{hpp,cpp}` & `ui/ControlRibbon.{hpp,cpp}` — drag-and-drop placement of arbitrary components onto the workspace.
- `ui/Workspace.{hpp,cpp}` — schematic/grid canvas mode or full drag-and-drop breadboard editing (move/delete components).
- `storage/FileIO.{hpp,cpp}` & `ui/SaveLoadDialog.{hpp,cpp}` — JSON circuit serialization (Save/Load).
- Live `Clock` signal generator UI on the breadboard.

## Quick Commands
- **Build & Run GUI (PowerShell / MSYS2 UCRT64):**
  ```powershell
  g++ -std=c++17 -I. main.cpp core/Pin.cpp core/Component.cpp core/Wire.cpp core/NetworkManager.cpp core/CircuitProject.cpp core/ICChip.cpp core/BreadboardCircuit.cpp gates/LogicGate.cpp gates/AndGate.cpp gates/OrGate.cpp gates/NotGate.cpp gates/XorGate.cpp io/IONode.cpp io/Switch.cpp io/PushButton.cpp io/LED.cpp io/Clock.cpp io/PowerSupply.cpp hardware/Draw.cpp hardware/BreadboardView.cpp hardware/ICView.cpp hardware/LEDView.cpp hardware/WireView.cpp hardware/PowerSupplyView.cpp hardware/PushButtonView.cpp ui/Application.cpp -lsfml-graphics -lsfml-window -lsfml-system -o circuit_sim.exe
  .\circuit_sim.exe
  ```
- **Run Headless Simulation Tests:**
  ```powershell
  g++ -std=c++17 -Wall -Wextra -Wpedantic -I. tests/sim_test.cpp core/Pin.cpp core/Component.cpp core/Wire.cpp core/NetworkManager.cpp core/CircuitProject.cpp core/ICChip.cpp core/BreadboardCircuit.cpp gates/LogicGate.cpp gates/AndGate.cpp gates/OrGate.cpp gates/NotGate.cpp gates/XorGate.cpp io/IONode.cpp io/Switch.cpp io/PushButton.cpp io/LED.cpp io/Clock.cpp io/PowerSupply.cpp -o sim_test.exe
  .\sim_test.exe
  ```
