#include "hardware/BreadboardView.hpp"
#include "hardware/LEDView.hpp"
#include "hardware/Theme.hpp"

int main() {
    ContextSettings settings;
    settings.antiAliasingLevel = 8;
    RenderWindow window(VideoMode({720, 420}), "LED preview - click an LED to toggle",
                        Style::Default, State::Windowed, settings);
    window.setFramerateLimit(60);

    BreadboardView board({52, 36});
    LEDView leds[4] = {
        LEDView(board, 32, ROW_J, LEDColor::Red, true),
        LEDView(board, 34, ROW_J, LEDColor::Red, false),
        LEDView(board, 36, ROW_J, LEDColor::Green, true),
        LEDView(board, 38, ROW_J, LEDColor::Green, false)};
    bool lit[4] = {true, false, true, false};

    View zoom(Vector2f(740, 290), Vector2f(180, 105));
    window.setView(zoom);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<Event::Closed>()) window.close();
            if (auto click = event->getIf<Event::MouseButtonPressed>()) {
                Vector2f p = window.mapPixelToCoords(click->position);
                for (int i = 0; i < 4; i++) {
                    Vector2f a = leds[i].anodePos();
                    if (abs(p.x - (a.x + PITCH / 2)) < 12 && p.y > a.y) {
                        lit[i] = !lit[i];
                        leds[i].setOn(lit[i]);
                    }
                }
            }
        }

        window.clear(BACKGROUND);
        board.draw(window);
        for (int i = 0; i < 4; i++) leds[i].draw(window);
        window.display();
    }
}
