#include "hardware/BreadboardView.hpp"
#include "hardware/Draw.hpp"
#include "hardware/Theme.hpp"

static void drawHole(RenderTarget& target, Vector2f p) {
    drawRect(target, p.x - HOLE_SIZE / 2, p.y - HOLE_SIZE / 2, HOLE_SIZE, HOLE_SIZE, HOLE_COLOR);
}

static void drawSign(RenderTarget& target, float x, float y, bool plus, Color color) {
    drawRect(target, x - SIGN_SIZE / 2, y - SIGN_THICKNESS / 2, SIGN_SIZE, SIGN_THICKNESS, color);
    if (plus)
        drawRect(target, x - SIGN_THICKNESS / 2, y - SIGN_SIZE / 2, SIGN_THICKNESS, SIGN_SIZE, color);
}

BreadboardView::BreadboardView(Vector2f position) : origin(position) {}

Vector2f BreadboardView::holePos(int col, int row) {
    float y = FIRST_ROW_Y + row * PITCH;
    if (row >= ROW_F) y = SECOND_HALF_Y + (row - ROW_F) * PITCH;
    return {origin.x + LEFT_MARGIN + col * PITCH, origin.y + y};
}

Vector2f BreadboardView::railPos(int col, int rail) {
    return {origin.x + LEFT_MARGIN + col * PITCH, origin.y + RAIL_Y[rail]};
}

void BreadboardView::draw(RenderTarget& target) {
    drawRect(target, origin.x, origin.y, BOARD_WIDTH, BOARD_HEIGHT, BOARD_COLOR);

    float gapTop = holePos(0, ROW_E).y + GAP_MARGIN;
    float gapBottom = holePos(0, ROW_F).y - GAP_MARGIN;
    drawRect(target, origin.x, gapTop, BOARD_WIDTH, gapBottom - gapTop, GAP_COLOR);

    for (int i = 0; i < 4; i++) {
        Color color = (i % 2 == 0) ? RAIL_RED : RAIL_BLUE;
        drawRect(target, origin.x + RAIL_LINE_MARGIN, origin.y + RAIL_LINE_Y[i],
                 RAIL_LINE_LENGTH, RAIL_LINE_THICKNESS, color);
    }

    for (int rail = TOP_PLUS; rail <= BOTTOM_MINUS; rail++) {
        bool plus = (rail == TOP_PLUS || rail == BOTTOM_PLUS);
        Color color = plus ? RAIL_RED : RAIL_BLUE;
        float y = origin.y + RAIL_Y[rail];
        drawSign(target, origin.x + SIGN_X, y, plus, color);
        drawSign(target, origin.x + BOARD_WIDTH - SIGN_X, y, plus, color);
    }

    for (int col = 0; col < COLUMNS; col++) {
        for (int row = ROW_A; row <= ROW_J; row++)
            drawHole(target, holePos(col, row));

        bool gapBetweenGroups = (col % RAIL_GROUP == RAIL_GROUP - 1);
        if (!gapBetweenGroups) {
            for (int rail = TOP_PLUS; rail <= BOTTOM_MINUS; rail++)
                drawHole(target, railPos(col, rail));
        }
    }
}
