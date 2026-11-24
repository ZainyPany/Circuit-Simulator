# Simple Digital Logic Circuit Simulator (`simple_demo/`)

A beginner-friendly, modular **C++17 + SFML 3.0** simulator split cleanly into `.hpp` and `.cpp` files.  
It uses a clean blank canvas (no breadboard) with **4 authentic 14-pin DIP TTL ICs**, a **5V power supply**, **3 push buttons**, and **Red / Green / Blue LEDs**.

## Authentic 14-Pin DIP ICs Included
All four standard 74xx ICs come in real **14-pin DIP** packages (Pins `1–7` on the bottom, Pins `14–8` on the top; **Pin 14 = VCC (+5V)**, **Pin 7 = GND (0V)**):
- **`7408 AND`** — Quad 2-input AND (`(1,2→3)`, `(4,5→6)`, `(9,10→8)`, `(12,13→11)`)
- **`7402 NOR`** — Quad 2-input NOR (`(2,3→1)`, `(5,6→4)`, `(8,9→10)`, `(11,12→13)`)
- **`7404 NOT`** — Hex Inverter (`(1→2)`, `(3→4)`, `(5→6)`, `(9→8)`, `(11→10)`, `(13→12)`)
- **`7486 XOR`** — Quad 2-input XOR (`(1,2→3)`, `(4,5→6)`, `(9,10→8)`, `(12,13→11)`)

> Hover your mouse over **any leg** to see its exact pin number, gate input/output role, and live logic level in the bottom status bar. Output legs also have a **gold outline** so you can spot them immediately.

## File Structure (`.hpp` & `.cpp`)
- `DrawHelpers.hpp` / `DrawHelpers.cpp` — `distanceBetween`, `drawCenteredText`, `drawThickLine`
- `Pin.hpp` — `PinRole` (`Signal`, `Vcc`, `Gnd`) and `struct Pin`
- `Wire.hpp` / `Wire.cpp` — `struct Wire` (`propagate()` and `draw()`)
- `Component.hpp` / `Component.cpp` — abstract OOP base `class Component`
- `Components.hpp` / `Components.cpp` — `PowerSupplyUnit`, `PushButton`, `GateIC` (14-pin DIP), `LED`
- `main.cpp` — canvas setup, `simulateCircuit()`, interactive wiring loop, and rendering

## Build & Run Command (PowerShell / MSYS2 UCRT64)

Run from the root `Circuit-Simulator/` folder:

```powershell
g++ -std=c++17 simple_demo/main.cpp simple_demo/DrawHelpers.cpp simple_demo/Wire.cpp simple_demo/Component.cpp simple_demo/Components.cpp -lsfml-graphics -lsfml-window -lsfml-system -o simple_demo.exe; .\simple_demo.exe
```

## Quick Demo Steps
1. Power an IC: wire **`+5V`** (from `5V SUPPLY`) to **Pin 14 (`VCC`)**, and **`GND`** to **Pin 7 (`GND`)** — or click **`Auto-Power ICs`** in the top bar! Watch the IC's right-side power dot turn **green**.
2. Wire **`BTN A`** → **`7408 AND` Pin 1**, **`BTN B`** → **`7408 AND` Pin 2**, and **`7408 AND` Pin 3** → **`RED LED`**.
3. Click **`BTN A`** and **`BTN B`** to test all 4 truth-table states (`00`, `01`, `10`, `11`).
4. Click **`5V SUPPLY`** to turn power OFF/ON and show all ICs and LEDs reacting in real time.
