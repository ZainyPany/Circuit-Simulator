#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "core/BreadboardCircuit.hpp"
#include "hardware/BreadboardView.hpp"
#include "hardware/LEDView.hpp"
#include "hardware/PowerSupplyView.hpp"

struct UIButton {
    sf::FloatRect bounds;
    std::string label;
};

class Application {
private:
    static const unsigned int APP_WIDTH = 1060;
    static const unsigned int APP_HEIGHT = 510;

    sf::RenderWindow window;
    sf::Font font;
    bool fontLoaded;

    BreadboardCircuit circuit;
    BreadboardView board;

    // Interactive wiring state
    bool hasPendingWireStart;
    BoardPoint pendingWireStart;
    JumperColor activeWireColor;

    // Hover state
    bool hasHoverPoint;
    BoardPoint hoverPoint;
    sf::Vector2f mouseWorldPos;

    // Momentary right-click button index (-1 if none)
    int activeMomentaryButton;

    // Top bar buttons
    UIButton preset1Btn;
    UIButton preset2Btn;
    UIButton preset3Btn;
    UIButton undoWireBtn;
    UIButton resetBtn;
    UIButton colorBtns[5];

    bool loadSystemFont();
    void initToolbar();

    sf::Vector2f pointToScreen(const BoardPoint& pt);
    bool findClosestBoardPoint(sf::Vector2f worldPos, BoardPoint& outPoint);
    sf::Color toSfmlWireColor(JumperColor c) const;
    LEDColor toHardwareLedColor(LED::Color c) const;

    void handleMousePress(const sf::Event::MouseButtonPressed& click);
    void handleMouseRelease(const sf::Event::MouseButtonReleased& release);
    void handleKeyPress(const sf::Event::KeyPressed& key);
    void handleKeyRelease(const sf::Event::KeyReleased& key);

    void drawToolbar();
    void drawBreadboardContents();
    void drawStatusBar();

public:
    Application();
    int run();
};
