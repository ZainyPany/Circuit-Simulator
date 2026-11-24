#include <cassert>
#include <iostream>
#include <memory>
#include "core/BreadboardCircuit.hpp"
#include "core/CircuitProject.hpp"
#include "gates/AndGate.hpp"
#include "gates/NotGate.hpp"
#include "gates/OrGate.hpp"
#include "gates/XorGate.hpp"
#include "io/Clock.hpp"
#include "io/LED.hpp"
#include "io/PowerSupply.hpp"
#include "io/PushButton.hpp"
#include "io/Switch.hpp"

static void testCoreSimulationEngine() {
    CircuitProject project("Unit Test Circuit");

    Switch* swA = static_cast<Switch*>(project.addComponent(std::unique_ptr<Component>(new Switch("A", false))));
    Switch* swB = static_cast<Switch*>(project.addComponent(std::unique_ptr<Component>(new Switch("B", false))));
    AndGate* andGate = static_cast<AndGate*>(project.addComponent(std::unique_ptr<Component>(new AndGate())));
    NotGate* notGate = static_cast<NotGate*>(project.addComponent(std::unique_ptr<Component>(new NotGate())));
    LED* redLed = static_cast<LED*>(project.addComponent(std::unique_ptr<Component>(new LED(LED::Color::Red, "NAND_LED"))));

    project.addWire(swA->getOutputPin(), andGate->getInputPins()[0]);
    project.addWire(swB->getOutputPin(), andGate->getInputPins()[1]);
    project.addWire(andGate->getOutputPin(), notGate->getInputPins()[0]);
    project.addWire(notGate->getOutputPin(), redLed->getInputPin());

    // 0 NAND 0 = 1
    project.update();
    assert(redLed->isOn() == true);

    // 1 NAND 0 = 1
    swA->setState(true);
    project.update();
    assert(redLed->isOn() == true);

    // 1 NAND 1 = 0
    swB->setState(true);
    project.update();
    assert(redLed->isOn() == false);

    // Pause should freeze updates
    project.pause();
    swB->setState(false);
    project.update();
    assert(redLed->isOn() == false);

    project.start();
    project.update();
    assert(redLed->isOn() == true);

    std::cout << "[PASS] Core simulation engine (CircuitProject, Gates, Wires, NetworkManager)\n";
}

static void testPreset1BasicGates() {
    BreadboardCircuit circuit;
    circuit.loadPreset(DemoPreset::BasicGates);

    assert(circuit.getICs().size() == 4);
    assert(circuit.getButtons().size() == 3);
    assert(circuit.getLEDs().size() == 3);
    assert(circuit.getPowerSupply().isEnabled() == true);

    // All 4 ICs should be powered
    for (std::size_t i = 0; i < circuit.getICs().size(); ++i) {
        assert(circuit.getICs()[i].isPowered() == true);
    }

    // Truth table check: Red = AND, Green = OR, Blue = XOR
    const bool expected[4][3] = {
        {false, false, false}, // A=0, B=0
        {false, true,  true},  // A=0, B=1
        {false, true,  true},  // A=1, B=0
        {true,  true,  false}  // A=1, B=1
    };

    for (int a = 0; a <= 1; ++a) {
        for (int b = 0; b <= 1; ++b) {
            circuit.getButtons()[0].button.setLatched(a == 1);
            circuit.getButtons()[1].button.setLatched(b == 1);
            circuit.simulate();

            int row = a * 2 + b;
            assert(circuit.getLEDs()[0].led.isOn() == expected[row][0]); // Red (AND)
            assert(circuit.getLEDs()[1].led.isOn() == expected[row][1]); // Green (OR)
            assert(circuit.getLEDs()[2].led.isOn() == expected[row][2]); // Blue (XOR)
        }
    }

    // Check 7404 NOT responds to BTN C (Pin 1 = col 8 bottom, Pin 2 = col 9 bottom)
    circuit.getButtons()[2].button.setLatched(false);
    circuit.simulate();
    assert(circuit.isPointHigh(BoardPoint::Hole(9, 6)) == true); // NOT(0) = 1

    circuit.getButtons()[2].button.setLatched(true);
    circuit.simulate();
    assert(circuit.isPointHigh(BoardPoint::Hole(9, 6)) == false); // NOT(1) = 0

    std::cout << "[PASS] Preset 1 (7408 AND -> Red, 7432 OR -> Green, 7486 XOR -> Blue, 7404 NOT)\n";
}

static void testPreset2AllFourICsInverted() {
    BreadboardCircuit circuit;
    circuit.loadPreset(DemoPreset::InvertedGates);

    // Truth table check: Red = NAND, Green = NOR, Blue = XNOR (all 4 ICs active)
    const bool expected[4][3] = {
        {true,  true,  true},  // A=0, B=0
        {true,  false, false}, // A=0, B=1
        {true,  false, false}, // A=1, B=0
        {false, false, true}   // A=1, B=1
    };

    for (int a = 0; a <= 1; ++a) {
        for (int b = 0; b <= 1; ++b) {
            circuit.getButtons()[0].button.setLatched(a == 1);
            circuit.getButtons()[1].button.setLatched(b == 1);
            circuit.simulate();

            int row = a * 2 + b;
            assert(circuit.getLEDs()[0].led.isOn() == expected[row][0]); // Red (NAND)
            assert(circuit.getLEDs()[1].led.isOn() == expected[row][1]); // Green (NOR)
            assert(circuit.getLEDs()[2].led.isOn() == expected[row][2]); // Blue (XNOR)
        }
    }

    // Power supply OFF -> all ICs lose power, all LEDs turn OFF
    circuit.getPowerSupply().setEnabled(false);
    circuit.simulate();
    assert(circuit.getPowerSupply().getOutputVoltage() == 0.0f);
    for (std::size_t i = 0; i < circuit.getICs().size(); ++i) {
        assert(circuit.getICs()[i].isPowered() == false);
    }
    for (std::size_t i = 0; i < circuit.getLEDs().size(); ++i) {
        assert(circuit.getLEDs()[i].led.isOn() == false);
    }

    // Power supply ON again -> restored
    circuit.getPowerSupply().setEnabled(true);
    circuit.simulate();
    assert(circuit.getPowerSupply().getOutputVoltage() == 5.0f);
    assert(circuit.getLEDs()[2].led.isOn() == true); // A=1, B=1 => XNOR=1

    std::cout << "[PASS] Preset 2 (All 4 ICs: NAND/NOR/XNOR + 5V Power Supply ON/OFF)\n";
}

static void testSandboxInteractiveWiring() {
    BreadboardCircuit circuit;
    circuit.loadPreset(DemoPreset::Sandbox);

    // No LEDs lit initially
    assert(circuit.getLEDs()[0].led.isOn() == false);
    assert(circuit.getLEDs()[1].led.isOn() == false);
    assert(circuit.getLEDs()[2].led.isOn() == false);

    // Wire BTN C output (col 39 top) -> 7404 NOT Pin 1 (col 8 bottom)
    // Wire 7404 NOT Pin 2 (col 9 bottom) -> Blue LED anode (col 38 bottom)
    circuit.addWire(BoardPoint::Hole(39, 3), BoardPoint::Hole(8, 7), JumperColor::Green);
    circuit.addWire(BoardPoint::Hole(9, 8), BoardPoint::Hole(38, 8), JumperColor::Blue);

    // BTN C unpressed (0) -> NOT -> 1 -> Blue LED ON
    assert(circuit.getLEDs()[2].led.isOn() == true);

    // Press BTN C momentarily -> NOT -> 0 -> Blue LED OFF
    circuit.getButtons()[2].button.press();
    circuit.simulate();
    assert(circuit.getLEDs()[2].led.isOn() == false);

    circuit.getButtons()[2].button.release();
    circuit.simulate();
    assert(circuit.getLEDs()[2].led.isOn() == true);

    // Undo last wire -> Blue LED turns OFF
    circuit.removeLastWire();
    assert(circuit.getLEDs()[2].led.isOn() == false);

    std::cout << "[PASS] Sandbox interactive wiring & undo\n";
}

int main() {
    testCoreSimulationEngine();
    testPreset1BasicGates();
    testPreset2AllFourICsInverted();
    testSandboxInteractiveWiring();
    std::cout << "All interim demo simulation tests passed!\n";
    return 0;
}
