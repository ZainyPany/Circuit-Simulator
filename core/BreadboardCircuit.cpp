#include "core/BreadboardCircuit.hpp"
#include <algorithm>

namespace {
// Row indices matching hardware/Theme.hpp
const int B_ROW_A = 0;
const int B_ROW_B = 1;
const int B_ROW_C = 2;
const int B_ROW_D = 3;
const int B_ROW_E = 4;
const int B_ROW_F = 5;
const int B_ROW_G = 6;
const int B_ROW_H = 7;
const int B_ROW_I = 8;
const int B_ROW_J = 9;

const int B_TOP_PLUS = 0;
const int B_TOP_MINUS = 1;
const int B_BOTTOM_PLUS = 2;
const int B_BOTTOM_MINUS = 3;

struct DisjointSet {
    int parent[BreadboardCircuit::TOTAL_NODES];

    void init() {
        for (int i = 0; i < BreadboardCircuit::TOTAL_NODES; ++i) {
            parent[i] = i;
        }
    }

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }

    void unite(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra != rb) {
            parent[rb] = ra;
        }
    }
};
} // namespace

BreadboardCircuit::BreadboardCircuit()
    : powerSupply(5.0f, true, "5V DC"), currentPreset(DemoPreset::BasicGates) {
    for (int i = 0; i < TOTAL_NODES; ++i) {
        nodeHigh[i] = false;
        nodeGnd[i] = false;
        nodeVcc[i] = false;
    }
    setupBaseComponents();
    loadPreset(DemoPreset::BasicGates);
}

void BreadboardCircuit::setupBaseComponents() {
    ics.clear();
    ics.push_back(ICChip(ICType::IC7408_AND, 0));
    ics.push_back(ICChip(ICType::IC7404_NOT, 8));
    ics.push_back(ICChip(ICType::IC7432_OR, 16));
    ics.push_back(ICChip(ICType::IC7486_XOR, 24));

    buttons.clear();
    buttons.push_back(BreadboardButton(32, B_ROW_C, "BTN A"));
    buttons.push_back(BreadboardButton(35, B_ROW_C, "BTN B"));
    buttons.push_back(BreadboardButton(38, B_ROW_C, "BTN C"));

    leds.clear();
    leds.push_back(BreadboardLED(LED::Color::Red,   32, B_ROW_J, "RED"));
    leds.push_back(BreadboardLED(LED::Color::Green, 35, B_ROW_J, "GRN"));
    leds.push_back(BreadboardLED(LED::Color::Blue,  38, B_ROW_J, "BLU"));
}

void BreadboardCircuit::addBasePowerWires() {
    // Bridge top power rails to bottom power rails at column 39 (or 31)
    wires.push_back({BoardPoint::Rail(31, B_TOP_PLUS),  BoardPoint::Rail(31, B_BOTTOM_PLUS),  JumperColor::Red,   true});
    wires.push_back({BoardPoint::Rail(32, B_TOP_MINUS), BoardPoint::Rail(32, B_BOTTOM_MINUS), JumperColor::Black, true});

    // Power all 4 ICs: Pin 14 (firstCol, ROW_E) -> TOP_PLUS, Pin 7 (firstCol + 6, ROW_F) -> BOTTOM_MINUS
    for (std::size_t i = 0; i < ics.size(); ++i) {
        int c = ics[i].getFirstCol();
        wires.push_back({BoardPoint::Rail(c, B_TOP_PLUS),         BoardPoint::Hole(c, B_ROW_A),     JumperColor::Red,   true});
        wires.push_back({BoardPoint::Rail(c + 6, B_BOTTOM_MINUS), BoardPoint::Hole(c + 6, B_ROW_J), JumperColor::Black, true});
    }

    // Feed +5V into the left leg of each push button (avoid rail gap cols where col % 6 == 5)
    for (std::size_t i = 0; i < buttons.size(); ++i) {
        int c = buttons[i].col;
        int railCol = (c % 6 == 5) ? (c - 1) : c;
        wires.push_back({BoardPoint::Rail(railCol, B_TOP_PLUS), BoardPoint::Hole(c, B_ROW_A), JumperColor::Red, true});
    }

    // Connect each LED's cathode (col + 1) to GND (BOTTOM_MINUS)
    for (std::size_t i = 0; i < leds.size(); ++i) {
        int cathodeCol = leds[i].col + 1;
        int railCol = (cathodeCol % 6 == 5) ? (cathodeCol - 1) : cathodeCol;
        wires.push_back({BoardPoint::Hole(cathodeCol, B_ROW_I), BoardPoint::Rail(railCol, B_BOTTOM_MINUS), JumperColor::Black, true});
    }
}

void BreadboardCircuit::loadPreset(DemoPreset preset) {
    currentPreset = preset;
    wires.clear();
    addBasePowerWires();

    // Button output columns (top half strips):
    // BTN A output = col 33 (ROW_A..E)
    // BTN B output = col 36 (ROW_A..E)
    // BTN C output = col 39 (ROW_A..E)
    //
    // IC pin columns:
    // 7408 AND (col 0):  Pin 1=col 0 (F..J), Pin 2=col 1 (F..J), Pin 3=col 2 (F..J)
    // 7404 NOT (col 8):  Pin 1=col 8 (F..J), Pin 2=col 9 (F..J), Pin 3=col 10, Pin 4=col 11, Pin 5=col 12, Pin 6=col 13
    // 7432 OR  (col 16): Pin 1=col 16 (F..J), Pin 2=col 17 (F..J), Pin 3=col 18 (F..J)
    // 7486 XOR (col 24): Pin 1=col 24 (F..J), Pin 2=col 25 (F..J), Pin 3=col 26 (F..J)
    //
    // LED anodes (bottom half strips):
    // RED   LED anode = col 32 (F..J)
    // GREEN LED anode = col 35 (F..J)
    // BLUE  LED anode = col 38 (F..J)

    if (preset == DemoPreset::BasicGates) {
        // BTN A -> AND(1), OR(1), XOR(1)
        wires.push_back({BoardPoint::Hole(33, B_ROW_B), BoardPoint::Hole(0,  B_ROW_G), JumperColor::Yellow, true});
        wires.push_back({BoardPoint::Hole(0,  B_ROW_H), BoardPoint::Hole(16, B_ROW_H), JumperColor::Yellow, true});
        wires.push_back({BoardPoint::Hole(16, B_ROW_I), BoardPoint::Hole(24, B_ROW_I), JumperColor::Yellow, true});

        // BTN B -> AND(2), OR(2), XOR(2)
        wires.push_back({BoardPoint::Hole(36, B_ROW_B), BoardPoint::Hole(1,  B_ROW_G), JumperColor::Blue, true});
        wires.push_back({BoardPoint::Hole(1,  B_ROW_H), BoardPoint::Hole(17, B_ROW_H), JumperColor::Blue, true});
        wires.push_back({BoardPoint::Hole(17, B_ROW_I), BoardPoint::Hole(25, B_ROW_I), JumperColor::Blue, true});

        // BTN C -> 7404 NOT Pin 1 (so 7404 also responds live to BTN C)
        wires.push_back({BoardPoint::Hole(39, B_ROW_D), BoardPoint::Hole(8, B_ROW_G), JumperColor::Green, true});

        // Outputs -> LEDs: AND -> Red, OR -> Green, XOR -> Blue
        wires.push_back({BoardPoint::Hole(2,  B_ROW_J), BoardPoint::Hole(32, B_ROW_H), JumperColor::Red,   true});
        wires.push_back({BoardPoint::Hole(18, B_ROW_J), BoardPoint::Hole(35, B_ROW_H), JumperColor::Green, true});
        wires.push_back({BoardPoint::Hole(26, B_ROW_J), BoardPoint::Hole(38, B_ROW_H), JumperColor::Blue,  true});
    } else if (preset == DemoPreset::InvertedGates) {
        // BTN A -> AND(1), OR(1), XOR(1)
        wires.push_back({BoardPoint::Hole(33, B_ROW_B), BoardPoint::Hole(0,  B_ROW_G), JumperColor::Yellow, true});
        wires.push_back({BoardPoint::Hole(0,  B_ROW_H), BoardPoint::Hole(16, B_ROW_H), JumperColor::Yellow, true});
        wires.push_back({BoardPoint::Hole(16, B_ROW_I), BoardPoint::Hole(24, B_ROW_I), JumperColor::Yellow, true});

        // BTN B -> AND(2), OR(2), XOR(2)
        wires.push_back({BoardPoint::Hole(36, B_ROW_B), BoardPoint::Hole(1,  B_ROW_G), JumperColor::Blue, true});
        wires.push_back({BoardPoint::Hole(1,  B_ROW_H), BoardPoint::Hole(17, B_ROW_H), JumperColor::Blue, true});
        wires.push_back({BoardPoint::Hole(17, B_ROW_I), BoardPoint::Hole(25, B_ROW_I), JumperColor::Blue, true});

        // Gate outputs -> 7404 NOT inputs (pins 1, 3, 5 = cols 8, 10, 12)
        wires.push_back({BoardPoint::Hole(2,  B_ROW_G), BoardPoint::Hole(8,  B_ROW_G), JumperColor::Red,   true});
        wires.push_back({BoardPoint::Hole(18, B_ROW_G), BoardPoint::Hole(10, B_ROW_G), JumperColor::Green, true});
        wires.push_back({BoardPoint::Hole(26, B_ROW_G), BoardPoint::Hole(12, B_ROW_G), JumperColor::Blue,  true});

        // 7404 NOT outputs (pins 2, 4, 6 = cols 9, 11, 13) -> LEDs (NAND->Red, NOR->Green, XNOR->Blue)
        wires.push_back({BoardPoint::Hole(9,  B_ROW_J), BoardPoint::Hole(32, B_ROW_H), JumperColor::Red,   true});
        wires.push_back({BoardPoint::Hole(11, B_ROW_J), BoardPoint::Hole(35, B_ROW_H), JumperColor::Green, true});
        wires.push_back({BoardPoint::Hole(13, B_ROW_J), BoardPoint::Hole(38, B_ROW_H), JumperColor::Blue,  true});
    }
    // DemoPreset::Sandbox leaves only power/ground wires so the user can wire freely.

    simulate();
}

DemoPreset BreadboardCircuit::getCurrentPreset() const {
    return currentPreset;
}

std::string BreadboardCircuit::getPresetName() const {
    switch (currentPreset) {
        case DemoPreset::BasicGates:    return "Preset 1: AND / OR / XOR";
        case DemoPreset::InvertedGates: return "Preset 2: NAND / NOR / XNOR (All 4 ICs)";
        case DemoPreset::Sandbox:       return "Preset 3: Free Wiring Sandbox";
    }
    return "Custom";
}

int BreadboardCircuit::stripNode(int col, bool topHalf) {
    if (col < 0) col = 0;
    if (col >= BOARD_COLS) col = BOARD_COLS - 1;
    return topHalf ? col : (BOARD_COLS + col);
}

int BreadboardCircuit::railNode(int railIndex) {
    if (railIndex < 0) railIndex = 0;
    if (railIndex > 3) railIndex = 3;
    return 2 * BOARD_COLS + railIndex;
}

int BreadboardCircuit::pointToNode(const BoardPoint& pt) {
    if (pt.isRail) {
        return railNode(pt.index);
    }
    bool topHalf = (pt.index <= B_ROW_E);
    return stripNode(pt.col, topHalf);
}

void BreadboardCircuit::simulate() {
    powerSupply.evaluate();

    DisjointSet dsu;
    dsu.init();

    // 1. Unite nodes connected by jumper wires
    for (std::size_t i = 0; i < wires.size(); ++i) {
        int a = pointToNode(wires[i].from);
        int b = pointToNode(wires[i].to);
        dsu.unite(a, b);
    }

    // 2. Unite nodes bridged by currently pressed push buttons
    for (std::size_t i = 0; i < buttons.size(); ++i) {
        if (buttons[i].button.isPressed()) {
            bool topHalf = (buttons[i].row <= B_ROW_E);
            int leftNode = stripNode(buttons[i].col, topHalf);
            int rightNode = stripNode(buttons[i].col + 1, topHalf);
            dsu.unite(leftNode, rightNode);
        }
    }

    bool compVcc[TOTAL_NODES];
    bool compGnd[TOTAL_NODES];
    bool compHigh[TOTAL_NODES];

    for (int i = 0; i < TOTAL_NODES; ++i) {
        compVcc[i] = false;
        compGnd[i] = false;
        compHigh[i] = false;
    }

    // 3. Seed 5V power supply onto TOP_PLUS (+5V) and TOP_MINUS (GND)
    if (powerSupply.isEnabled()) {
        int vccRoot = dsu.find(railNode(B_TOP_PLUS));
        int gndRoot = dsu.find(railNode(B_TOP_MINUS));
        compVcc[vccRoot] = true;
        compHigh[vccRoot] = true;
        compGnd[gndRoot] = true;
    }

    // 4. Iteratively resolve cascading IC outputs
    for (int iter = 0; iter < 16; ++iter) {
        bool nextHigh[TOTAL_NODES];
        for (int i = 0; i < TOTAL_NODES; ++i) {
            nextHigh[i] = compVcc[i];
        }

        for (std::size_t c = 0; c < ics.size(); ++c) {
            ICChip& chip = ics[c];

            int vccNode = stripNode(chip.pinColumn(14), chip.pinIsTopHalf(14));
            int gndNode = stripNode(chip.pinColumn(7),  chip.pinIsTopHalf(7));
            bool hasVcc = compVcc[dsu.find(vccNode)];
            bool hasGnd = compGnd[dsu.find(gndNode)];

            bool pinIn[15];
            bool pinOut[15];
            bool pinDriven[15];
            pinIn[0] = false;
            for (int p = 1; p <= 14; ++p) {
                int n = stripNode(chip.pinColumn(p), chip.pinIsTopHalf(p));
                pinIn[p] = compHigh[dsu.find(n)];
            }

            chip.evaluate(hasVcc, hasGnd, pinIn, pinOut, pinDriven);

            for (int p = 1; p <= 14; ++p) {
                if (pinDriven[p] && pinOut[p]) {
                    int n = stripNode(chip.pinColumn(p), chip.pinIsTopHalf(p));
                    nextHigh[dsu.find(n)] = true;
                }
            }
        }

        bool changed = false;
        for (int i = 0; i < TOTAL_NODES; ++i) {
            if (compHigh[i] != nextHigh[i]) {
                compHigh[i] = nextHigh[i];
                changed = true;
            }
        }
        if (!changed) break;
    }

    // 5. Record per-node states
    for (int i = 0; i < TOTAL_NODES; ++i) {
        int r = dsu.find(i);
        nodeVcc[i] = compVcc[r];
        nodeGnd[i] = compGnd[r];
        nodeHigh[i] = compHigh[r];
    }

    // 6. Update LEDs: lit only when supply is ON, anode is HIGH, and cathode is connected to GND
    for (std::size_t i = 0; i < leds.size(); ++i) {
        bool topHalf = (leds[i].row <= B_ROW_E);
        int anodeNode = stripNode(leds[i].col, topHalf);
        int cathodeNode = stripNode(leds[i].col + 1, topHalf);
        bool lit = powerSupply.isEnabled() && nodeHigh[anodeNode] && nodeGnd[cathodeNode];
        leds[i].led.setLit(lit);
    }
}

void BreadboardCircuit::addWire(const BoardPoint& from, const BoardPoint& to, JumperColor color) {
    if (from == to) return;
    wires.push_back({from, to, color, true});
    simulate();
}

bool BreadboardCircuit::removeLastWire() {
    if (wires.empty()) return false;
    wires.pop_back();
    simulate();
    return true;
}

bool BreadboardCircuit::removeWireAt(std::size_t index) {
    if (index >= wires.size()) return false;
    wires.erase(wires.begin() + static_cast<std::ptrdiff_t>(index));
    simulate();
    return true;
}

void BreadboardCircuit::clearSignalWires() {
    wires.clear();
    addBasePowerWires();
    simulate();
}

PowerSupply& BreadboardCircuit::getPowerSupply() {
    return powerSupply;
}

const PowerSupply& BreadboardCircuit::getPowerSupply() const {
    return powerSupply;
}

std::vector<ICChip>& BreadboardCircuit::getICs() {
    return ics;
}

const std::vector<ICChip>& BreadboardCircuit::getICs() const {
    return ics;
}

std::vector<BreadboardButton>& BreadboardCircuit::getButtons() {
    return buttons;
}

const std::vector<BreadboardButton>& BreadboardCircuit::getButtons() const {
    return buttons;
}

std::vector<BreadboardLED>& BreadboardCircuit::getLEDs() {
    return leds;
}

const std::vector<BreadboardLED>& BreadboardCircuit::getLEDs() const {
    return leds;
}

const std::vector<JumperWire>& BreadboardCircuit::getWires() const {
    return wires;
}

bool BreadboardCircuit::isNodeHigh(int nodeId) const {
    if (nodeId < 0 || nodeId >= TOTAL_NODES) return false;
    return nodeHigh[nodeId];
}

bool BreadboardCircuit::isNodeGnd(int nodeId) const {
    if (nodeId < 0 || nodeId >= TOTAL_NODES) return false;
    return nodeGnd[nodeId];
}

bool BreadboardCircuit::isNodeVcc(int nodeId) const {
    if (nodeId < 0 || nodeId >= TOTAL_NODES) return false;
    return nodeVcc[nodeId];
}

bool BreadboardCircuit::isPointHigh(const BoardPoint& pt) const {
    return isNodeHigh(pointToNode(pt));
}

std::string BreadboardCircuit::describePoint(const BoardPoint& pt) const {
    int nodeId = pointToNode(pt);
    std::string state = nodeHigh[nodeId] ? "HIGH (5V)" : (nodeGnd[nodeId] ? "GND (0V)" : "LOW (0V)");

    if (pt.isRail) {
        const char* railNames[4] = {"Top +5V Rail", "Top GND Rail", "Bottom +5V Rail", "Bottom GND Rail"};
        int idx = (pt.index >= 0 && pt.index < 4) ? pt.index : 0;
        return std::string(railNames[idx]) + " | " + state;
    }

    // Check if this column/half matches an IC pin
    bool topHalf = (pt.index <= B_ROW_E);
    for (std::size_t i = 0; i < ics.size(); ++i) {
        for (int p = 1; p <= 14; ++p) {
            if (ics[i].pinColumn(p) == pt.col && ics[i].pinIsTopHalf(p) == topHalf) {
                return ics[i].pinDescription(p) + " | " + state;
            }
        }
    }

    // Check buttons
    for (std::size_t i = 0; i < buttons.size(); ++i) {
        if (topHalf && pt.col == buttons[i].col) {
            return buttons[i].label + " In | " + state;
        }
        if (topHalf && pt.col == buttons[i].col + 1) {
            return buttons[i].label + " Out | " + state;
        }
    }

    // Check LEDs
    for (std::size_t i = 0; i < leds.size(); ++i) {
        if (!topHalf && pt.col == leds[i].col) {
            return leds[i].label + " LED Anode (+) | " + state;
        }
        if (!topHalf && pt.col == leds[i].col + 1) {
            return leds[i].label + " LED Cathode (-) | " + state;
        }
    }

    char rowChar = static_cast<char>('A' + pt.index);
    return "Col " + std::to_string(pt.col + 1) + " Row " + std::string(1, rowChar) + " | " + state;
}
