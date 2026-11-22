#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

// Every size, name and colour of the hardware GUI lives here.

// window
const int WINDOW_WIDTH = 700;
const int WINDOW_HEIGHT = 400;
const char* const FONT_PATH = "C:/Windows/Fonts/arial.ttf";

// breadboard
const int COLUMNS = 30;
const float PITCH = 18;               // distance between two holes
const float HOLE_SIZE = 7;
const float BOARD_WIDTH = 596;
const float BOARD_HEIGHT = 328;
const float LEFT_MARGIN = 37;         // board edge to the first column
const float FIRST_ROW_Y = 74;         // y of row a
const float SECOND_HALF_Y = 182;      // y of row f
const float GAP_MARGIN = 7;           // middle gap and chip body start this far from rows e and f
const int RAIL_GROUP = 6;             // rail holes: groups of 5, then 1 space
const float RAIL_LINE_MARGIN = 14;
const float RAIL_LINE_LENGTH = 568;
const float RAIL_LINE_THICKNESS = 2;
const float RAIL_LINE_Y[4] = {9, 49, 279, 319};   // red, blue, red, blue

// row names
const int ROW_A = 0;
const int ROW_B = 1;
const int ROW_C = 2;
const int ROW_D = 3;
const int ROW_E = 4;
const int ROW_F = 5;
const int ROW_G = 6;
const int ROW_H = 7;
const int ROW_I = 8;
const int ROW_J = 9;

// power rail names
const int TOP_PLUS = 0;
const int TOP_MINUS = 1;
const int BOTTOM_PLUS = 2;
const int BOTTOM_MINUS = 3;
const float RAIL_Y[4] = {20, 38, 290, 308};

// chip
const float CHIP_OVERHANG = 11;       // body sticks out past the first and last pin
const float PIN_WIDTH = 6;
const float PIN_HEIGHT = 10;
const float PIN_OVERSHOOT = 1;        // pin goes slightly past the hole centre
const float NUMBER_OFFSET = 8;        // pin number distance from its hole
const int NUMBER_SIZE = 9;
const int LABEL_SIZE = 11;
const float LABEL_SHIFT = 6;          // moves the label away from the pin 1 dot
const float DOT_RADIUS = 2.5f;
const float DOT_INSET_X = 8;
const float DOT_INSET_Y = 13;

// wire
const float WIRE_THICKNESS = 5;

// colours
const Color BACKGROUND(49, 54, 70);
const Color BOARD_COLOR(245, 245, 245);
const Color GAP_COLOR(232, 232, 232);
const Color HOLE_COLOR(60, 60, 70);
const Color RAIL_RED(210, 60, 60);
const Color RAIL_BLUE(60, 90, 200);
const Color PIN_COLOR(170, 172, 180);
const Color CHIP_COLOR(45, 48, 56);
const Color DOT_COLOR(30, 32, 38);
const Color LABEL_COLOR(225, 225, 230);
const Color NUMBER_COLOR(90, 90, 90);
const Color RED_WIRE(220, 50, 47);
const Color BLACK_WIRE(35, 35, 40);
const Color YELLOW_WIRE(240, 190, 40);
const Color BLUE_WIRE(50, 120, 230);
const Color GREEN_WIRE(40, 170, 90);
