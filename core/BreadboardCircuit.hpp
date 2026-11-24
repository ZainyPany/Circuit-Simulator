#pragma once
#include <string>
#include <vector>
#include "core/ICChip.hpp"
#include "io/LED.hpp"
#include "io/PowerSupply.hpp"
#include "io/PushButton.hpp"

// Breadboard coordinate point: either a main strip hole (col 0..39, row 0..9)
// or a power rail hole (col 0..39, rail 0..3).
struct BoardPoint {
    bool isRail;
    int col;
    int index; // row (0..9) if !isRail, rail (0..3: TOP_PLUS..BOTTOM_MINUS) if isRail

    static BoardPoint Hole(int c, int r) {
        BoardPoint p;
        p.isRail = false;
        p.col = c;
        p.index = r;
        return p;
    }

    static BoardPoint Rail(int c, int railIdx) {
        BoardPoint p;
        p.isRail = true;
        p.col = c;
        p.index = railIdx;
        return p;
    }

    bool operator==(const BoardPoint& other) const {
        return isRail == other.isRail && col == other.col && index == other.index;
    }
};

enum class JumperColor {
    Red,
    Black,
    Yellow,
    Blue,
    Green
};

struct JumperWire {
    BoardPoint from;
    BoardPoint to;
    JumperColor color;
    bool removable; // false for permanent supply-to-rail leads if desired, true for breadboard wires
};

struct BreadboardLED {
    LED led;
    int col; // anode at (col, row), cathode at (col + 1, row)
    int row;
    std::string label;

    BreadboardLED(LED::Color c, int column, int rowIdx, const std::string& lbl)
        : led(c, lbl, column, rowIdx), col(column), row(rowIdx), label(lbl) {}
};

struct BreadboardButton {
    PushButton button;
    int col; // left leg at (col, row), right leg at (col + 1, row)
    int row;
    std::string label;

    BreadboardButton(int column, int rowIdx, const std::string& lbl)
        : button(lbl, column, rowIdx), col(column), row(rowIdx), label(lbl) {}
};

enum class DemoPreset {
    BasicGates,    // 7408 AND -> Red, 7432 OR -> Green, 7486 XOR -> Blue (+ 7404 NOT powered)
    InvertedGates, // All 4 ICs active: AND+NOT=NAND -> Red, OR+NOT=NOR -> Green, XOR+NOT=XNOR -> Blue
    Sandbox        // Power + ICs + Buttons + LEDs ready for custom manual wiring
};

// Pure C++17 electrical breadboard simulator (no SFML dependency).
class BreadboardCircuit {
public:
    static const int BOARD_COLS = 40;
    static const int TOTAL_NODES = 84; // 0..39 top strips (A-E), 40..79 bottom strips (F-J), 80..83 rails

private:
    PowerSupply powerSupply;
    std::vector<ICChip> ics;
    std::vector<BreadboardButton> buttons;
    std::vector<BreadboardLED> leds;
    std::vector<JumperWire> wires;
    DemoPreset currentPreset;

    // Last simulated state per breadboard node (0..83)
    bool nodeHigh[TOTAL_NODES];
    bool nodeGnd[TOTAL_NODES];
    bool nodeVcc[TOTAL_NODES];

    void setupBaseComponents();
    void addBasePowerWires();

public:
    BreadboardCircuit();

    void loadPreset(DemoPreset preset);
    DemoPreset getCurrentPreset() const;
    std::string getPresetName() const;

    // Electrical node mapping
    static int pointToNode(const BoardPoint& pt);
    static int stripNode(int col, bool topHalf);
    static int railNode(int railIndex);

    // Run full electrical & logic simulation
    void simulate();

    // Wire editing
    void addWire(const BoardPoint& from, const BoardPoint& to, JumperColor color);
    bool removeLastWire();
    bool removeWireAt(std::size_t index);
    void clearSignalWires();

    // Accessors
    PowerSupply& getPowerSupply();
    const PowerSupply& getPowerSupply() const;

    std::vector<ICChip>& getICs();
    const std::vector<ICChip>& getICs() const;

    std::vector<BreadboardButton>& getButtons();
    const std::vector<BreadboardButton>& getButtons() const;

    std::vector<BreadboardLED>& getLEDs();
    const std::vector<BreadboardLED>& getLEDs() const;

    const std::vector<JumperWire>& getWires() const;

    bool isNodeHigh(int nodeId) const;
    bool isNodeGnd(int nodeId) const;
    bool isNodeVcc(int nodeId) const;
    bool isPointHigh(const BoardPoint& pt) const;
    std::string describePoint(const BoardPoint& pt) const;
};
