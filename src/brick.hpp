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

    Vector2 get_position() {
        return { m_rect.x, m_rect.y };
    }

    Vector2 where_circle_hit(const Vector2 position, const float radius) {
        const Vector2 top_left = { m_rect.x, m_rect.y };
        const Vector2 top_right = { m_rect.x + m_rect.width, m_rect.y };
        const Vector2 bottom_left = { m_rect.x, m_rect.y + m_rect.height };
        const Vector2 bottom_right = { m_rect.x + m_rect.width, m_rect.y + m_rect.height };

        // Top
        if (CheckCollisionCircleLine(position, radius, top_left, top_right)) return { 0, -1 };
        // Bottom
        if (CheckCollisionCircleLine(position, radius, bottom_left, bottom_right)) return { 0, 1 };
        // Left
        if (CheckCollisionCircleLine(position, radius, top_left, bottom_left)) return { -1, 0 };
        // Right
        if (CheckCollisionCircleLine(position, radius, top_right, bottom_right)) return { 1, 0 };

        return { 0, 0 }; // shouldn't be reachable
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
