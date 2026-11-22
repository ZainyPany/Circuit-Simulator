#include "hardware/BreadboardView.hpp"
#include "hardware/ICView.hpp"
#include "hardware/LEDView.hpp"
#include "hardware/WireView.hpp"
#include "hardware/Theme.hpp"

int main() {
    ContextSettings settings;
    settings.antiAliasingLevel = 8;
    RenderWindow window(VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Hardware GUI preview",
                        Style::Default, State::Windowed, settings);
    window.setFramerateLimit(60);

    Font font;
    if (!font.openFromFile(FONT_PATH)) return 1;

    BreadboardView board({52, 36});
    ICView andChip(board, font, 0, "7408 AND");
    ICView notChip(board, font, 8, "7404 NOT");
    ICView orChip(board, font, 16, "7432 OR");
    ICView xorChip(board, font, 24, "7486 XOR");

    LEDView led1(board, 32, ROW_J, LEDColor::Red, true);
    LEDView led2(board, 34, ROW_J, LEDColor::Green, false);
    LEDView led3(board, 36, ROW_J, LEDColor::Yellow, true);
    LEDView led4(board, 38, ROW_J, LEDColor::Blue, false);

    WireView vcc1(board.railPos(0, TOP_PLUS), board.holePos(0, ROW_A), RED_WIRE);
    WireView vcc2(board.railPos(8, TOP_PLUS), board.holePos(8, ROW_A), RED_WIRE);
    WireView vcc3(board.railPos(16, TOP_PLUS), board.holePos(16, ROW_A), RED_WIRE);
    WireView vcc4(board.railPos(24, TOP_PLUS), board.holePos(24, ROW_A), RED_WIRE);
    WireView gnd1(board.railPos(6, BOTTOM_MINUS), board.holePos(6, ROW_J), BLACK_WIRE);
    WireView gnd2(board.railPos(14, BOTTOM_MINUS), board.holePos(14, ROW_J), BLACK_WIRE);
    WireView gnd3(board.railPos(22, BOTTOM_MINUS), board.holePos(22, ROW_J), BLACK_WIRE);
    WireView gnd4(board.railPos(30, BOTTOM_MINUS), board.holePos(30, ROW_J), BLACK_WIRE);

    WireView out1(board.holePos(2, ROW_G), board.holePos(32, ROW_I), YELLOW_WIRE);
    WireView out2(board.holePos(10, ROW_H), board.holePos(34, ROW_I), GREEN_WIRE);
    WireView out3(board.holePos(18, ROW_I), board.holePos(36, ROW_I), BLUE_WIRE);
    WireView out4(board.holePos(26, ROW_J), board.holePos(38, ROW_I), YELLOW_WIRE);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<Event::Closed>()) window.close();
        }

        window.clear(BACKGROUND);
        board.draw(window);
        andChip.draw(window);
        notChip.draw(window);
        orChip.draw(window);
        xorChip.draw(window);
        led1.draw(window);
        led2.draw(window);
        led3.draw(window);
        led4.draw(window);
        vcc1.draw(window);
        vcc2.draw(window);
        vcc3.draw(window);
        vcc4.draw(window);
        gnd1.draw(window);
        gnd2.draw(window);
        gnd3.draw(window);
        gnd4.draw(window);
        out1.draw(window);
        out2.draw(window);
        out3.draw(window);
        out4.draw(window);
        window.display();
    }
}
