#include "BreadboardView.hpp"
#include "Draw.hpp"
#include "Theme.hpp"

// Draw a single pin hole as a small square rectangle
static void drawHole(RenderTarget& target, Vector2f p) {
    drawRect(target, p.x - HOLE_SIZE / 2.0f, p.y - HOLE_SIZE / 2.0f, HOLE_SIZE, HOLE_SIZE, HOLE_COLOR);
}

// Draw a '+' or '-' symbol using 1 or 2 small rectangles
static void drawSign(RenderTarget& target, float x, float y, bool plus, Color color) {
    // Horizontal rectangle (used for both '+' and '-')
    drawRect(target, x - SIGN_SIZE / 2.0f, y - SIGN_THICKNESS / 2.0f, SIGN_SIZE, SIGN_THICKNESS, color);
    
    // Vertical rectangle (only added if it is a '+')
    if (plus) {
        drawRect(target, x - SIGN_THICKNESS / 2.0f, y - SIGN_SIZE / 2.0f, SIGN_THICKNESS, SIGN_SIZE, color);
    }
}

BreadboardView::BreadboardView(Vector2f position) : origin(position) {}

// Calculate the (x, y) center coordinate of a hole at (col, row)
Vector2f BreadboardView::holePos(int col, int row) {
    // Top half: Rows A through E
    float y = FIRST_ROW_Y + row * PITCH;
    // Bottom half: Rows F through J (shifted down past the center divider)
    if (row >= ROW_F) {
        y = SECOND_HALF_Y + (row - ROW_F) * PITCH;
    }
    return {origin.x + LEFT_MARGIN + col * PITCH, origin.y + y};
}

// Calculate the (x, y) center coordinate of a power rail hole
Vector2f BreadboardView::railPos(int col, int rail) {
    return {origin.x + LEFT_MARGIN + col * PITCH, origin.y + RAIL_Y[rail]};
}

void BreadboardView::draw(RenderTarget& target) {
    // 1. Draw the breadboard base (large cream rectangle)
    drawRect(target, origin.x, origin.y, BOARD_WIDTH, BOARD_HEIGHT, BOARD_COLOR);

    // 2. Draw the center divider groove (gray rectangle between row E and row F)
    float gapTop = holePos(0, ROW_E).y + GAP_MARGIN;
    float gapBottom = holePos(0, ROW_F).y - GAP_MARGIN;
    drawRect(target, origin.x, gapTop, BOARD_WIDTH, gapBottom - gapTop, GAP_COLOR);

    // 3. Draw the 4 colored power rail stripes (red for +, blue for -)
    for (int i = 0; i < 4; i++) {
        Color color = (i % 2 == 0) ? RAIL_RED : RAIL_BLUE;
        drawRect(target, origin.x + RAIL_LINE_MARGIN, origin.y + RAIL_LINE_Y[i],
                 RAIL_LINE_LENGTH, RAIL_LINE_THICKNESS, color);
    }

    // 4. Draw the '+' and '-' signs on both ends of each power rail
    for (int rail = TOP_PLUS; rail <= BOTTOM_MINUS; rail++) {
        bool plus = (rail == TOP_PLUS || rail == BOTTOM_PLUS);
        Color color = plus ? RAIL_RED : RAIL_BLUE;
        float y = origin.y + RAIL_Y[rail];
        drawSign(target, origin.x + SIGN_X, y, plus, color);
        drawSign(target, origin.x + BOARD_WIDTH - SIGN_X, y, plus, color);
    }

    // 5. Draw the holes (small rectangles) across all columns
    for (int col = 0; col < COLUMNS; col++) {
        // Main tie-point holes (Row A to Row J)
        for (int row = ROW_A; row <= ROW_J; row++) {
            drawHole(target, holePos(col, row));
        }

        // Power rail holes (skip every 6th column to create 5-hole groups)
        bool gapBetweenGroups = (col % RAIL_GROUP == RAIL_GROUP - 1);
        if (!gapBetweenGroups) {
            for (int rail = TOP_PLUS; rail <= BOTTOM_MINUS; rail++) {
                drawHole(target, railPos(col, rail));
            }
        }
    }
}
