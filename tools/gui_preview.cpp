// Standalone program to look at the hardware GUI (breadboard, ICs, wires).
// It is NOT the real application. Build and run it with scripts/build_gui_preview.ps1
//
//   gui_preview.exe             opens a window
//   gui_preview.exe picture.png saves a picture instead of opening a window

#include <SFML/Graphics.hpp>
#include "ui/hardware/BreadboardView.hpp"
#include "ui/hardware/ICView.hpp"
#include "ui/hardware/WireView.hpp"
#include "ui/hardware/Theme.hpp"
using namespace sf;
using namespace Theme;

int main(int argc, char* argv[]) {
    ContextSettings settings;
    settings.antiAliasingLevel = 8;   // smooth edges

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

    auto drawAll = [&](RenderTarget& target) {
        target.clear(BACKGROUND);
        board.draw(target);
        andChip.draw(target);
        notChip.draw(target);
        bcdChip.draw(target);
        power.draw(target);
        ground.draw(target);
        signal1.draw(target);
        signal2.draw(target);
    };

    if (argc > 1) {   // save a picture and quit
        RenderTexture picture({WINDOW_WIDTH, WINDOW_HEIGHT}, settings);
        drawAll(picture);
        picture.display();
        bool saved = picture.getTexture().copyToImage().saveToFile(argv[1]);
        return saved ? 0 : 1;
    }

    RenderWindow window(VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Hardware GUI preview",
                        Style::Default, State::Windowed, settings);
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<Event::Closed>()) window.close();
        }
        drawAll(window);
        window.display();
    }
}
