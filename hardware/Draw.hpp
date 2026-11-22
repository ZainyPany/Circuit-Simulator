#pragma once
#include <string>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

void drawRect(RenderTarget& target, float x, float y, float w, float h, Color color);

void drawCircle(RenderTarget& target, float x, float y, float radius, Color color);
void drawText(RenderTarget& target, Font& font, string str, int size, Color color, float x, float y);
void drawLine(RenderTarget& target, Vector2f a, Vector2f b, float thickness, Color color);
