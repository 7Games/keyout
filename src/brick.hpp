#ifndef BRICK_HPP_
#define BRICK_HPP_

#include <optional>
#include <raylib.h>

#include "misc.hpp"

class Brick {
public:
    Brick(Vector2 const& position) {
        m_rect = {
            position.x * (BRICK_WIDTH + 5),
            position.y * (BRICK_HEIGHT + 5),
            BRICK_WIDTH,
            BRICK_HEIGHT,
        };
    }

    void draw() {
        DrawRectangleRec(m_rect, BRICK_COLOUR);
    }

    bool is_circle_inside(const Vector2 position, const float radius) {
        return CheckCollisionCircleRec(position,
                                       radius,
                                       m_rect);
    }
private:
    Rectangle m_rect;
};

#endif // BRICK_HPP_
