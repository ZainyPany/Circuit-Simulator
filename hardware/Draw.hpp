#pragma once
#include <string>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

// Small drawing helpers used by the hardware GUI.

void drawRect(RenderTarget& target, float x, float y, float w, float h, Color color);

void drawCircle(RenderTarget& target, float x, float y, float radius, Color color);

// Text whose middle is at (x, y).
void drawText(RenderTarget& target, Font& font, string str, int size, Color color, float x, float y);

// A thick line (SFML's own lines are always 1 pixel wide).
void drawLine(RenderTarget& target, Vector2f a, Vector2f b, float thickness, Color color);
