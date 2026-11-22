#include "ui/hardware/BreadboardView.hpp"
#include "ui/hardware/DrawHelpers.hpp"
#include "ui/hardware/Theme.hpp"
using namespace sf;
using namespace Theme;

BreadboardView::BreadboardView(Vector2f position) : origin(position) {}

Vector2f BreadboardView::holePos(int col, int row) const {
    float y;
    if (row < ROW_F) y = FIRST_ROW_Y + row * PITCH;
    else y = SECOND_HALF_Y + (row - ROW_F) * PITCH;
    return {origin.x + LEFT_MARGIN + col * PITCH, origin.y + y};
}

Vector2f BreadboardView::railPos(int col, int rail) const {
    return {origin.x + LEFT_MARGIN + col * PITCH, origin.y + RAIL_Y[rail]};
}

void BreadboardView::drawHole(RenderTarget& target, Vector2f position) const {
    drawRect(target, position.x - HOLE_SIZE / 2, position.y - HOLE_SIZE / 2,
             HOLE_SIZE, HOLE_SIZE, HOLE_COLOR);
}

void BreadboardView::drawRailLine(RenderTarget& target, float railY, float distance, Color color) const {
    drawRect(target, origin.x + RAIL_LINE_MARGIN, origin.y + railY + distance,
             RAIL_LINE_LENGTH, RAIL_LINE_THICKNESS, color);
}

void BreadboardView::draw(RenderTarget& target) const {
    drawRect(target, origin.x, origin.y, BOARD_WIDTH, BOARD_HEIGHT, BOARD_COLOR);

    // the middle gap sits between row e and row f
    float gapTop = holePos(0, ROW_E).y + GAP_MARGIN;
    float gapBottom = holePos(0, ROW_F).y - GAP_MARGIN;
    drawRect(target, origin.x, gapTop, BOARD_WIDTH, gapBottom - gapTop, GAP_COLOR);

    drawRailLine(target, RAIL_Y[TOP_PLUS], -RAIL_LINE_DISTANCE, RAIL_RED);
    drawRailLine(target, RAIL_Y[TOP_MINUS], RAIL_LINE_DISTANCE, RAIL_BLUE);
    drawRailLine(target, RAIL_Y[BOTTOM_PLUS], -RAIL_LINE_DISTANCE, RAIL_RED);
    drawRailLine(target, RAIL_Y[BOTTOM_MINUS], RAIL_LINE_DISTANCE, RAIL_BLUE);

    for (int col = 0; col < COLUMNS; col++) {
        for (int row = ROW_A; row <= ROW_J; row++)
            drawHole(target, holePos(col, row));

        bool spaceBetweenGroups = (col % RAIL_GROUP == RAIL_GROUP - 1);
        if (!spaceBetweenGroups) {
            for (int rail = TOP_PLUS; rail <= BOTTOM_MINUS; rail++)
                drawHole(target, railPos(col, rail));
        }
    }
}
