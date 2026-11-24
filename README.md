# Digital Logic Circuit Simulator

**Course:** CS/CE 224/272 — Object-Oriented Programming and Design Methodologies (Habib University, Fall 2026)  
**Team Members:**
- Muhammad Khan (11071) — `@mk11071`
- Abbas Huzaifa Dar (11013) — `@achdiop`
- Syed Zain Imran (11682) — `@ZainyPany`

---

## Overview

A 2D interactive **Digital Logic Circuit Simulator** written in **C++17** using **SFML 3.0**.  
The application models a physical 40-column solderless breadboard with real-time electrical connectivity, 74xx-series TTL integrated circuits, a 5V DC power supply, tactile push buttons, and color-coded LEDs.

## Interim Demo Features

- **5V DC Power Supply (`PowerSupply` / `PowerSupplyView`)**
  - Interactive ON/OFF power switch with live `5.00 V` / `0.00 V` readout and status indicator LED.
  - Feeds the breadboard `+5V` (red) and `GND` (blue/black) power rails.
  - Turning off the supply de-energizes all ICs and LEDs in real time.
- **4 Standard 14-Pin TTL ICs (`ICChip` / `ICView`)**
  - `7408` — Quad 2-Input **AND** Gate
  - `7404` — Hex **NOT** Inverter
  - `7432` — Quad 2-Input **OR** Gate
  - `7486` — Quad 2-Input **XOR** Gate
  - Internally composed of `AndGate`, `NotGate`, `OrGate`, and `XorGate` (`LogicGate` hierarchy).
  - Enforces realistic IC power checks: outputs are only driven when **Pin 14 (VCC)** is connected to `+5V` and **Pin 7 (GND)** is connected to `GND`.
- **Red, Green, and Blue LEDs (`LED` / `LEDView`)**
  - Mounted on the breadboard bottom strip with anode/cathode legs and glow rendering.
  - Lights up only when power is ON, the anode is driven HIGH (`5V`), and the cathode is tied to `GND`.
- **Interactive Push Buttons (`PushButton` / `PushButtonView`)**
  - 3 tactile breadboard push buttons (`BTN A`, `BTN B`, `BTN C`) bridging `+5V` to their output strips when pressed.
  - Supports **Left-click** to toggle/latch (easy multi-input testing with one mouse), **Right-click** hold for momentary press, and keyboard keys **`1`**, **`2`**, **`3`** for momentary hold.
- **Interactive Breadboard Wiring & Live Voltage Probe**
  - Click any two breadboard holes or rail holes to add a jumper wire in your selected color (`Red`, `Black`, `Yellow`, `Blue`, `Green`).
  - Hover over any hole or IC pin to inspect its pin description and live logic level (`HIGH (5V)` / `GND (0V)` / `LOW (0V)`).
  - Built-in presets for quick demonstrations:
    1. **Preset 1 (`AND / OR / XOR`)**: `BTN A` & `BTN B` drive `7408 AND` (Red LED), `7432 OR` (Green LED), `7486 XOR` (Blue LED), while `BTN C` drives `7404 NOT`.
    2. **Preset 2 (`NAND / NOR / XNOR`)**: Chains `7408`, `7432`, and `7486` through `7404 NOT` so **all 4 ICs** actively drive the Red, Green, and Blue LEDs simultaneously.
    3. **Preset 3 (`Sandbox`)**: Power rails, ICs, buttons, and LEDs are powered and ready for custom live wiring.

---

## Project Structure

```text
Circuit-Simulator/
├── main.cpp                    # Application entry point
├── core/                       # Core simulation engine & electrical breadboard model
│   ├── Pin.{hpp,cpp}
│   ├── Component.{hpp,cpp}
│   ├── Wire.{hpp,cpp}
│   ├── NetworkManager.{hpp,cpp}
│   ├── CircuitProject.{hpp,cpp}
│   ├── ICChip.{hpp,cpp}        # 14-pin 7408, 7404, 7432, 7486 IC models
│   └── BreadboardCircuit.{hpp,cpp}
├── gates/                      # Polymorphic LogicGate hierarchy
│   ├── LogicGate.{hpp,cpp}
│   ├── AndGate.{hpp,cpp}
│   ├── OrGate.{hpp,cpp}
│   ├── NotGate.{hpp,cpp}
│   └── XorGate.{hpp,cpp}
├── io/                         # Input/Output components
│   ├── IONode.{hpp,cpp}
│   ├── Switch.{hpp,cpp}
│   ├── PushButton.{hpp,cpp}
│   ├── LED.{hpp,cpp}
│   ├── Clock.{hpp,cpp}
│   └── PowerSupply.{hpp,cpp}
├── hardware/                   # SFML 3.0 breadboard visual components
│   ├── Theme.hpp
│   ├── Draw.{hpp,cpp}
│   ├── BreadboardView.{hpp,cpp}
│   ├── ICView.{hpp,cpp}
│   ├── LEDView.{hpp,cpp}
│   ├── WireView.{hpp,cpp}
│   ├── PowerSupplyView.{hpp,cpp}
│   └── PushButtonView.{hpp,cpp}
├── ui/                         # Main SFML application window & event handling
│   └── Application.{hpp,cpp}
├── storage/                    # Serialization stubs (upcoming milestone)
├── tests/
│   └── sim_test.cpp            # Headless C++17 unit tests (no SFML required)
└── docs/
    ├── ARCHITECTURE_AND_API.md # Engineering architecture, execution flow & function reference
    ├── Project_Proposal.pdf
    └── sfml-guide/
```

---

## Prerequisites

- **C++17 compiler** (`g++` via MSYS2 UCRT64 on Windows, or `g++` on Linux/WSL)
- **SFML 3.0** (`mingw-w64-ucrt-x86_64-sfml` in MSYS2 UCRT64, or SFML 3 built/installed on WSL/Linux — see `docs/sfml-guide/ucrt_sfml_setup with WSL-1.pdf`)

### Installing SFML 3.0 in MSYS2 (Windows)
Open the **MSYS2 MSYS** shell and run:
```bash
pacman -Syu
pacman -S --needed mingw-w64-ucrt-x86_64-toolchain
pacman -S mingw-w64-ucrt-x86_64-sfml
```

---

## Building & Running

### 1. Build the Interim Demo GUI (MSYS2 UCRT64 or WSL/Linux)

Run from the repository root (`Circuit-Simulator/`):

```bash
g++ -std=c++17 -I. \
  main.cpp \
  core/Pin.cpp core/Component.cpp core/Wire.cpp core/NetworkManager.cpp core/CircuitProject.cpp \
  core/ICChip.cpp core/BreadboardCircuit.cpp \
  gates/LogicGate.cpp gates/AndGate.cpp gates/OrGate.cpp gates/NotGate.cpp gates/XorGate.cpp \
  io/IONode.cpp io/Switch.cpp io/PushButton.cpp io/LED.cpp io/Clock.cpp io/PowerSupply.cpp \
  hardware/Draw.cpp hardware/BreadboardView.cpp hardware/ICView.cpp hardware/LEDView.cpp \
  hardware/WireView.cpp hardware/PowerSupplyView.cpp hardware/PushButtonView.cpp \
  ui/Application.cpp \
  -lsfml-graphics -lsfml-window -lsfml-system \
  -o circuit_sim.exe
```

On Linux/WSL, replace `-o circuit_sim.exe` with `-o circuit_sim` and run `./circuit_sim`.  
On Windows MSYS2 UCRT64, run:
```bash
./circuit_sim.exe
```

### 2. Run Headless Simulation Unit Tests (No SFML Required)

You can verify the core simulation engine, all 4 ICs, 5V power supply, push buttons, and Red/Green/Blue LED logic directly without SFML:

```bash
g++ -std=c++17 -Wall -Wextra -I. \
  tests/sim_test.cpp \
  core/Pin.cpp core/Component.cpp core/Wire.cpp core/NetworkManager.cpp core/CircuitProject.cpp \
  core/ICChip.cpp core/BreadboardCircuit.cpp \
  gates/LogicGate.cpp gates/AndGate.cpp gates/OrGate.cpp gates/NotGate.cpp gates/XorGate.cpp \
  io/IONode.cpp io/Switch.cpp io/PushButton.cpp io/LED.cpp io/Clock.cpp io/PowerSupply.cpp \
  -o sim_test.exe

./sim_test.exe
```

---

## Controls

| Action | Control |
|---|---|
| Toggle 5V Power Supply ON / OFF | Click **POWER** button on supply panel, or press `P` |
| Toggle Push Buttons (`BTN A`, `BTN B`, `BTN C`) | **Left-click** a button |
| Momentary Press Push Button | **Right-click & hold** a button, or hold keys `1`, `2`, `3` |
| Add Jumper Wire | Click start breadboard hole, then click destination hole |
| Cancel Wire Drawing | Press `Esc` or **Right-click** |
| Undo Last Jumper Wire | Click **Undo Wire** or press `Backspace` |
| Switch Demo Presets | Click top toolbar presets or press `F1`, `F2`, `F3` |
| Change Wire Color | Click color swatches (`R`, `K`, `Y`, `B`, `G`) in top toolbar |
| Inspect Live Node Voltage / Pin Function | Hover mouse over any breadboard hole or rail hole |
