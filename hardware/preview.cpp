// A small program to look at the hardware GUI. It is not the real application.
// Run it with run_preview.ps1

#include "hardware/BreadboardView.hpp"
#include "hardware/ICView.hpp"
#include "hardware/WireView.hpp"
#include "hardware/Theme.hpp"

int main() {
    ContextSettings settings;
    settings.antiAliasingLevel = 8;   // smooth edges
    RenderWindow window(VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Hardware GUI preview",
                        Style::Default, State::Windowed, settings);
    window.setFramerateLimit(60);

    Font font;
    if (!font.openFromFile(FONT_PATH)) return 1;

    BreadboardView board({52, 36});
    ICView andChip(board, font, 1, "7408 AND");
    ICView notChip(board, font, 10, "7404 NOT");
    ICView bcdChip(board, font, 20, "7447 BCD", 16);

    WireView power(board.railPos(0, TOP_PLUS), board.holePos(1, ROW_A), RED_WIRE);
    WireView ground(board.railPos(8, BOTTOM_MINUS), board.holePos(7, ROW_J), BLACK_WIRE);
    WireView signal1(board.holePos(3, ROW_J), board.holePos(10, ROW_G), YELLOW_WIRE);
    WireView signal2(board.holePos(11, ROW_I), board.holePos(20, ROW_H), BLUE_WIRE);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<Event::Closed>()) window.close();
        }

        window.clear(BACKGROUND);
        board.draw(window);
        andChip.draw(window);
        notChip.draw(window);
        bcdChip.draw(window);
        power.draw(window);
        ground.draw(window);
        signal1.draw(window);
        signal2.draw(window);
        window.display();
    }
}
