#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

// ============================================================================
// Window & Font Settings
// ============================================================================
const int WINDOW_WIDTH = 880;
const int WINDOW_HEIGHT = 400;
const char* const FONT_PATH = "C:/Windows/Fonts/arial.ttf";

// ============================================================================
// Breadboard Grid Dimensions
// ============================================================================
const int COLUMNS = 40;              // 40 columns of tie-point holes
const float PITCH = 18;              // Distance between adjacent hole centers
const float HOLE_SIZE = 7;           // Width and height of each square hole
const float BOARD_WIDTH = 776;       // Total breadboard width
const float BOARD_HEIGHT = 328;      // Total breadboard height
const float LEFT_MARGIN = 37;        // Left offset to first column
const float FIRST_ROW_Y = 74;        // Y position of Row A
const float SECOND_HALF_Y = 182;     // Y position of Row F (after center groove)
const float GAP_MARGIN = 7;          // Spacing around center groove

// ============================================================================
// Power Rail Constants
// ============================================================================
const int RAIL_GROUP = 6;            // Gap after every 5 holes
const float RAIL_LINE_MARGIN = 14;   // Left offset for colored rail lines
const float RAIL_LINE_LENGTH = 748;  // Length of colored rail lines
const float RAIL_LINE_THICKNESS = 2; // Thickness of rail stripe
const float RAIL_LINE_Y[4] = {9, 49, 279, 319}; // Y levels of colored rail lines

// Row Index Definitions (A to J)
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

// Power Rail Index Definitions
const int TOP_PLUS = 0;
const int TOP_MINUS = 1;
const int BOTTOM_PLUS = 2;
const int BOTTOM_MINUS = 3;
const float RAIL_Y[4] = {20, 38, 290, 308}; // Y positions of power rail holes

// Rail '+' and '-' Symbol Dimensions
const float SIGN_SIZE = 9;
const float SIGN_THICKNESS = 2;
const float SIGN_X = 22;

// ============================================================================
// IC (Integrated Circuit) Dimensions
// ============================================================================
const float CHIP_OVERHANG = 11;
const float PIN_WIDTH = 6;
const float PIN_HEIGHT = 10;
const float PIN_OVERSHOOT = 1;
const float NUMBER_OFFSET = 8;
const int NUMBER_SIZE = 9;
const int LABEL_SIZE = 11;
const float LABEL_SHIFT = 6;
const float DOT_RADIUS = 2.5f;
const float DOT_INSET_X = 8;
const float DOT_INSET_Y = 13;

// ============================================================================
// Wire & LED Dimensions
// ============================================================================
const float WIRE_THICKNESS = 5;

const float LED_LEG_WIDTH = 2;
const float LED_LEG_LENGTH = 9;
const float LED_BODY_RADIUS = 7;
const float LED_GLOW_RADIUS = 11;
const float LED_BASE_WIDTH = 16;
const float LED_BASE_HEIGHT = 3;

// ============================================================================
// Color Palette
// ============================================================================
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

// Wire Colors
const Color RED_WIRE(220, 50, 47);
const Color BLACK_WIRE(35, 35, 40);
const Color YELLOW_WIRE(240, 190, 40);
const Color BLUE_WIRE(50, 120, 230);
const Color GREEN_WIRE(40, 170, 90);

// LED State Colors (Lit vs Unlit)
const Color LED_RED_ON(255, 70, 70);
const Color LED_RED_OFF(130, 40, 40);
const Color LED_GREEN_ON(80, 240, 110);
const Color LED_GREEN_OFF(35, 100, 55);
const Color LED_YELLOW_ON(255, 225, 70);
const Color LED_YELLOW_OFF(120, 105, 40);
const Color LED_BLUE_ON(90, 160, 255);
const Color LED_BLUE_OFF(40, 65, 120);
