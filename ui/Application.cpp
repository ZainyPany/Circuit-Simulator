#include "ui/Application.hpp"
#include <cmath>
#include <iostream>
#include "hardware/Draw.hpp"
#include "hardware/ICView.hpp"
#include "hardware/LEDView.hpp"
#include "hardware/PushButtonView.hpp"
#include "hardware/Theme.hpp"
#include "hardware/WireView.hpp"

namespace {
const float BOARD_ORIGIN_X = 248.0f;
const float BOARD_ORIGIN_Y = 82.0f;
const float SUPPLY_ORIGIN_X = 28.0f;
const float SUPPLY_ORIGIN_Y = 120.0f;
const float TOOLBAR_HEIGHT = 54.0f;
const float STATUSBAR_Y = 462.0f;
} // namespace

Application::Application()
    : fontLoaded(false),
      board({BOARD_ORIGIN_X, BOARD_ORIGIN_Y}),
      hasPendingWireStart(false),
      pendingWireStart(BoardPoint::Hole(0, 0)),
      activeWireColor(JumperColor::Yellow),
      hasHoverPoint(false),
      hoverPoint(BoardPoint::Hole(0, 0)),
      mouseWorldPos({0.0f, 0.0f}),
      activeMomentaryButton(-1) {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    window.create(sf::VideoMode({APP_WIDTH, APP_HEIGHT}),
                  "Digital Logic Circuit Simulator - Interim Demo",
                  sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    fontLoaded = loadSystemFont();
    initToolbar();
    circuit.simulate();
}

bool Application::loadSystemFont() {
    const char* candidates[] = {
        FONT_PATH,
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/consola.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/usr/share/fonts/TTF/DejaVuSans.ttf"
    };

    for (int i = 0; i < 6; ++i) {
        if (font.openFromFile(candidates[i])) {
            return true;
        }
    }
    return false;
}

void Application::initToolbar() {
    preset1Btn  = {sf::FloatRect({20.0f, 11.0f},  {155.0f, 32.0f}), "1: AND/OR/XOR"};
    preset2Btn  = {sf::FloatRect({185.0f, 11.0f}, {180.0f, 32.0f}), "2: NAND/NOR/XNOR"};
    preset3Btn  = {sf::FloatRect({375.0f, 11.0f}, {120.0f, 32.0f}), "3: Sandbox"};

    const char* colorLabels[5] = {"R", "K", "Y", "B", "G"};
    for (int i = 0; i < 5; ++i) {
        colorBtns[i] = {sf::FloatRect({545.0f + i * 34.0f, 13.0f}, {28.0f, 28.0f}), colorLabels[i]};
    }

    undoWireBtn = {sf::FloatRect({755.0f, 11.0f}, {125.0f, 32.0f}), "Undo Wire"};
    resetBtn    = {sf::FloatRect({892.0f, 11.0f}, {140.0f, 32.0f}), "Reset Preset"};
}

sf::Vector2f Application::pointToScreen(const BoardPoint& pt) {
    if (pt.isRail) {
        return board.railPos(pt.col, pt.index);
    }
    return board.holePos(pt.col, pt.index);
}

bool Application::findClosestBoardPoint(sf::Vector2f worldPos, BoardPoint& outPoint) {
    float bestDistSq = 11.0f * 11.0f;
    bool found = false;

    for (int col = 0; col < COLUMNS; ++col) {
        for (int row = ROW_A; row <= ROW_J; ++row) {
            sf::Vector2f p = board.holePos(col, row);
            float dx = worldPos.x - p.x;
            float dy = worldPos.y - p.y;
            float d2 = dx * dx + dy * dy;
            if (d2 < bestDistSq) {
                bestDistSq = d2;
                outPoint = BoardPoint::Hole(col, row);
                found = true;
            }
        }

        bool gapBetweenGroups = (col % RAIL_GROUP == RAIL_GROUP - 1);
        if (!gapBetweenGroups) {
            for (int rail = TOP_PLUS; rail <= BOTTOM_MINUS; ++rail) {
                sf::Vector2f p = board.railPos(col, rail);
                float dx = worldPos.x - p.x;
                float dy = worldPos.y - p.y;
                float d2 = dx * dx + dy * dy;
                if (d2 < bestDistSq) {
                    bestDistSq = d2;
                    outPoint = BoardPoint::Rail(col, rail);
                    found = true;
                }
            }
        }
    }
    return found;
}

sf::Color Application::toSfmlWireColor(JumperColor c) const {
    switch (c) {
        case JumperColor::Red:    return RED_WIRE;
        case JumperColor::Black:  return BLACK_WIRE;
        case JumperColor::Yellow: return YELLOW_WIRE;
        case JumperColor::Blue:   return BLUE_WIRE;
        case JumperColor::Green:  return GREEN_WIRE;
    }
    return YELLOW_WIRE;
}

LEDColor Application::toHardwareLedColor(LED::Color c) const {
    switch (c) {
        case LED::Color::Red:    return LEDColor::Red;
        case LED::Color::Green:  return LEDColor::Green;
        case LED::Color::Blue:   return LEDColor::Blue;
        case LED::Color::Yellow: return LEDColor::Yellow;
    }
    return LEDColor::Red;
}

void Application::handleMousePress(const sf::Event::MouseButtonPressed& click) {
    sf::Vector2f pos = window.mapPixelToCoords(click.position);

    if (click.button == sf::Mouse::Button::Right) {
        // Cancel pending wire if drawing
        if (hasPendingWireStart) {
            hasPendingWireStart = false;
            return;
        }

        // Right-click on a push button = momentary press while held
        std::vector<BreadboardButton>& btns = circuit.getButtons();
        for (std::size_t i = 0; i < btns.size(); ++i) {
            PushButtonView view(board, font, btns[i].col, btns[i].row, btns[i].label, btns[i].button.isPressed());
            if (view.contains(pos)) {
                btns[i].button.press();
                activeMomentaryButton = static_cast<int>(i);
                circuit.simulate();
                return;
            }
        }
        return;
    }

    if (click.button != sf::Mouse::Button::Left) {
        return;
    }

    // 1. Check toolbar buttons
    if (preset1Btn.bounds.contains(pos)) {
        hasPendingWireStart = false;
        circuit.loadPreset(DemoPreset::BasicGates);
        return;
    }
    if (preset2Btn.bounds.contains(pos)) {
        hasPendingWireStart = false;
        circuit.loadPreset(DemoPreset::InvertedGates);
        return;
    }
    if (preset3Btn.bounds.contains(pos)) {
        hasPendingWireStart = false;
        circuit.loadPreset(DemoPreset::Sandbox);
        return;
    }
    if (undoWireBtn.bounds.contains(pos)) {
        hasPendingWireStart = false;
        circuit.removeLastWire();
        return;
    }
    if (resetBtn.bounds.contains(pos)) {
        hasPendingWireStart = false;
        circuit.loadPreset(circuit.getCurrentPreset());
        return;
    }

    const JumperColor palette[5] = {
        JumperColor::Red, JumperColor::Black, JumperColor::Yellow, JumperColor::Blue, JumperColor::Green
    };
    for (int i = 0; i < 5; ++i) {
        if (colorBtns[i].bounds.contains(pos)) {
            activeWireColor = palette[i];
            return;
        }
    }

    // 2. Check 5V Power Supply switch
    PowerSupplyView supplyView({SUPPLY_ORIGIN_X, SUPPLY_ORIGIN_Y}, font, circuit.getPowerSupply().isEnabled());
    if (supplyView.containsSwitch(pos)) {
        circuit.getPowerSupply().toggle();
        circuit.simulate();
        return;
    }

    // 3. Check breadboard Push Buttons (Left-click toggles latched state)
    std::vector<BreadboardButton>& btns = circuit.getButtons();
    for (std::size_t i = 0; i < btns.size(); ++i) {
        PushButtonView view(board, font, btns[i].col, btns[i].row, btns[i].label, btns[i].button.isPressed());
        if (view.contains(pos)) {
            btns[i].button.toggleLatch();
            circuit.simulate();
            return;
        }
    }

    // 4. Check breadboard holes for interactive jumper wiring
    BoardPoint clickedPt = BoardPoint::Hole(0, 0);
    if (findClosestBoardPoint(pos, clickedPt)) {
        if (!hasPendingWireStart) {
            pendingWireStart = clickedPt;
            hasPendingWireStart = true;
        } else {
            if (!(pendingWireStart == clickedPt)) {
                circuit.addWire(pendingWireStart, clickedPt, activeWireColor);
            }
            hasPendingWireStart = false;
        }
    } else {
        hasPendingWireStart = false;
    }
}

void Application::handleMouseRelease(const sf::Event::MouseButtonReleased& release) {
    if (release.button == sf::Mouse::Button::Right && activeMomentaryButton >= 0) {
        std::vector<BreadboardButton>& btns = circuit.getButtons();
        if (activeMomentaryButton < static_cast<int>(btns.size())) {
            btns[static_cast<std::size_t>(activeMomentaryButton)].button.release();
            circuit.simulate();
        }
        activeMomentaryButton = -1;
    }
}

void Application::handleKeyPress(const sf::Event::KeyPressed& key) {
    std::vector<BreadboardButton>& btns = circuit.getButtons();
    if (key.code == sf::Keyboard::Key::Num1 && btns.size() >= 1) {
        btns[0].button.press();
        circuit.simulate();
    } else if (key.code == sf::Keyboard::Key::Num2 && btns.size() >= 2) {
        btns[1].button.press();
        circuit.simulate();
    } else if (key.code == sf::Keyboard::Key::Num3 && btns.size() >= 3) {
        btns[2].button.press();
        circuit.simulate();
    } else if (key.code == sf::Keyboard::Key::P) {
        circuit.getPowerSupply().toggle();
        circuit.simulate();
    } else if (key.code == sf::Keyboard::Key::F1) {
        hasPendingWireStart = false;
        circuit.loadPreset(DemoPreset::BasicGates);
    } else if (key.code == sf::Keyboard::Key::F2) {
        hasPendingWireStart = false;
        circuit.loadPreset(DemoPreset::InvertedGates);
    } else if (key.code == sf::Keyboard::Key::F3) {
        hasPendingWireStart = false;
        circuit.loadPreset(DemoPreset::Sandbox);
    } else if (key.code == sf::Keyboard::Key::Backspace) {
        hasPendingWireStart = false;
        circuit.removeLastWire();
    } else if (key.code == sf::Keyboard::Key::Escape) {
        hasPendingWireStart = false;
    }
}

void Application::handleKeyRelease(const sf::Event::KeyReleased& key) {
    std::vector<BreadboardButton>& btns = circuit.getButtons();
    if (key.code == sf::Keyboard::Key::Num1 && btns.size() >= 1) {
        btns[0].button.release();
        circuit.simulate();
    } else if (key.code == sf::Keyboard::Key::Num2 && btns.size() >= 2) {
        btns[1].button.release();
        circuit.simulate();
    } else if (key.code == sf::Keyboard::Key::Num3 && btns.size() >= 3) {
        btns[2].button.release();
        circuit.simulate();
    }
}

void Application::drawToolbar() {
    drawRect(window, 0.0f, 0.0f, static_cast<float>(APP_WIDTH), TOOLBAR_HEIGHT, Color(32, 36, 48));
    drawRect(window, 0.0f, TOOLBAR_HEIGHT - 2.0f, static_cast<float>(APP_WIDTH), 2.0f, Color(22, 25, 34));

    auto drawBtn = [&](const UIButton& btn, bool active) {
        Color border = active ? Color(90, 170, 255) : Color(65, 72, 92);
        Color fill = active ? Color(52, 90, 145) : Color(48, 53, 68);
        drawRect(window, btn.bounds.position.x, btn.bounds.position.y,
                 btn.bounds.size.x, btn.bounds.size.y, border);
        drawRect(window, btn.bounds.position.x + 2.0f, btn.bounds.position.y + 2.0f,
                 btn.bounds.size.x - 4.0f, btn.bounds.size.y - 4.0f, fill);
        drawText(window, font, btn.label, 12, Color(235, 238, 245),
                 btn.bounds.position.x + btn.bounds.size.x * 0.5f,
                 btn.bounds.position.y + btn.bounds.size.y * 0.5f);
    };

    DemoPreset cur = circuit.getCurrentPreset();
    drawBtn(preset1Btn, cur == DemoPreset::BasicGates);
    drawBtn(preset2Btn, cur == DemoPreset::InvertedGates);
    drawBtn(preset3Btn, cur == DemoPreset::Sandbox);
    drawBtn(undoWireBtn, false);
    drawBtn(resetBtn, false);

    drawText(window, font, "Wire:", 11, Color(185, 192, 210), 522.0f, 27.0f);

    const JumperColor palette[5] = {
        JumperColor::Red, JumperColor::Black, JumperColor::Yellow, JumperColor::Blue, JumperColor::Green
    };
    for (int i = 0; i < 5; ++i) {
        bool selected = (activeWireColor == palette[i]);
        Color border = selected ? Color(255, 255, 255) : Color(70, 76, 94);
        drawRect(window, colorBtns[i].bounds.position.x, colorBtns[i].bounds.position.y,
                 colorBtns[i].bounds.size.x, colorBtns[i].bounds.size.y, border);
        drawRect(window, colorBtns[i].bounds.position.x + 3.0f, colorBtns[i].bounds.position.y + 3.0f,
                 colorBtns[i].bounds.size.x - 6.0f, colorBtns[i].bounds.size.y - 6.0f,
                 toSfmlWireColor(palette[i]));
    }
}

void Application::drawBreadboardContents() {
    // 1. 5V Power Supply panel + power leads into breadboard rails
    PowerSupplyView supplyView({SUPPLY_ORIGIN_X, SUPPLY_ORIGIN_Y}, font, circuit.getPowerSupply().isEnabled());
    supplyView.draw(window);

    WireView vccLead(supplyView.vccTerminalPos(), board.railPos(0, TOP_PLUS), RED_WIRE);
    WireView gndLead(supplyView.gndTerminalPos(), board.railPos(1, TOP_MINUS), BLACK_WIRE);
    vccLead.draw(window);
    gndLead.draw(window);

    // 2. Breadboard
    board.draw(window);

    // 3. 4 TTL ICs
    const std::vector<ICChip>& ics = circuit.getICs();
    for (std::size_t i = 0; i < ics.size(); ++i) {
        ICView icView(board, font, ics[i].getFirstCol(), ics[i].getLabel(), ics[i].getPinCount());
        icView.draw(window);

        // Small power status indicator on the right end of each IC body
        sf::Vector2f p7 = icView.pinPos(7);
        sf::Vector2f p8 = icView.pinPos(8);
        float indicatorX = p7.x + 4.0f;
        float indicatorY = (p7.y + p8.y) * 0.5f;
        drawCircle(window, indicatorX, indicatorY, 3.0f,
                   ics[i].isPowered() ? LED_GREEN_ON : Color(120, 45, 45));
    }

    // 4. Push Buttons
    const std::vector<BreadboardButton>& btns = circuit.getButtons();
    for (std::size_t i = 0; i < btns.size(); ++i) {
        PushButtonView btnView(board, font, btns[i].col, btns[i].row, btns[i].label, btns[i].button.isPressed());
        btnView.draw(window);
    }

    // 5. Red, Green, Blue LEDs
    const std::vector<BreadboardLED>& leds = circuit.getLEDs();
    const char* preset1Labels[3] = {"AND", "OR", "XOR"};
    const char* preset2Labels[3] = {"NAND", "NOR", "XNOR"};
    for (std::size_t i = 0; i < leds.size(); ++i) {
        LEDView ledView(board, leds[i].col, leds[i].row,
                        toHardwareLedColor(leds[i].led.getColor()),
                        leds[i].led.isOn());
        ledView.draw(window);

        std::string caption = leds[i].label;
        if (i < 3 && circuit.getCurrentPreset() == DemoPreset::BasicGates) {
            caption = preset1Labels[i];
        } else if (i < 3 && circuit.getCurrentPreset() == DemoPreset::InvertedGates) {
            caption = preset2Labels[i];
        }

        sf::Vector2f anode = ledView.anodePos();
        sf::Vector2f cathode = ledView.cathodePos();
        drawText(window, font, caption, 10, Color(230, 234, 242),
                 (anode.x + cathode.x) * 0.5f, anode.y + 42.0f);
    }

    // 6. All breadboard jumper wires
    const std::vector<JumperWire>& wires = circuit.getWires();
    for (std::size_t i = 0; i < wires.size(); ++i) {
        sf::Vector2f a = pointToScreen(wires[i].from);
        sf::Vector2f b = pointToScreen(wires[i].to);
        WireView wv(a, b, toSfmlWireColor(wires[i].color));
        wv.draw(window);
    }

    // 7. Highlight hovered hole & live wire preview
    if (hasHoverPoint) {
        sf::Vector2f hp = pointToScreen(hoverPoint);
        bool high = circuit.isPointHigh(hoverPoint);
        drawCircle(window, hp.x, hp.y, 6.5f, high ? Color(255, 220, 80, 180) : Color(120, 190, 255, 160));
    }

    if (hasPendingWireStart) {
        sf::Vector2f startPos = pointToScreen(pendingWireStart);
        sf::Vector2f endPos = hasHoverPoint ? pointToScreen(hoverPoint) : mouseWorldPos;
        drawCircle(window, startPos.x, startPos.y, 6.0f, Color(255, 255, 255));
        WireView preview(startPos, endPos, toSfmlWireColor(activeWireColor));
        preview.draw(window);
    }
}

void Application::drawStatusBar() {
    drawRect(window, 0.0f, STATUSBAR_Y, static_cast<float>(APP_WIDTH),
             static_cast<float>(APP_HEIGHT) - STATUSBAR_Y, Color(30, 34, 46));
    drawRect(window, 0.0f, STATUSBAR_Y, static_cast<float>(APP_WIDTH), 2.0f, Color(20, 23, 32));

    std::string leftText = circuit.getPresetName();
    if (hasHoverPoint) {
        leftText = "Probe: " + circuit.describePoint(hoverPoint);
    }
    drawText(window, font, leftText, 12, Color(230, 235, 245), 240.0f, STATUSBAR_Y + 24.0f);

    std::string hint = "Click 5V/Buttons | Keys 1-3: Hold Btn | Click 2 Holes: Add Wire | Backspace: Undo";
    drawText(window, font, hint, 11, Color(165, 174, 194), 765.0f, STATUSBAR_Y + 24.0f);
}

int Application::run() {
    if (!fontLoaded) {
        std::cerr << "Error: Could not load a system font (tried " << FONT_PATH << " and fallbacks).\n";
        return 1;
    }

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (auto click = event->getIf<sf::Event::MouseButtonPressed>()) {
                handleMousePress(*click);
            } else if (auto release = event->getIf<sf::Event::MouseButtonReleased>()) {
                handleMouseRelease(*release);
            } else if (auto moved = event->getIf<sf::Event::MouseMoved>()) {
                mouseWorldPos = window.mapPixelToCoords(moved->position);
                hasHoverPoint = findClosestBoardPoint(mouseWorldPos, hoverPoint);
            } else if (auto key = event->getIf<sf::Event::KeyPressed>()) {
                handleKeyPress(*key);
            } else if (auto keyRel = event->getIf<sf::Event::KeyReleased>()) {
                handleKeyRelease(*keyRel);
            }
        }

        window.clear(BACKGROUND);
        drawToolbar();
        drawBreadboardContents();
        drawStatusBar();
        window.display();
    }

    return 0;
}
