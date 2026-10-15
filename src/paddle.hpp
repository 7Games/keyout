#ifndef PADDLE_HPP_
#define PADDLE_HPP_

#include <optional>
#include <raylib.h>
#include <raymath.h>

#include "misc.hpp"

class Paddle {
public:
    Paddle() {
        m_begin  = { 0, 0 };
        m_end    = { 0, 0 };
        m_active = false;
    }

    void move(Vector2 const& begin, Vector2 const& end) {
        m_begin = begin;
        m_end   = end;
    }

    void activate() {
        m_active = true;
    }
    void unactivate() {
        m_active = false;
    }

    std::optional<Vector2> get_normal() {
        if (m_active) {};

        const Vector2 paddle_vector = m_end - m_begin;
        const Vector2 paddle_normal = { -paddle_vector.y, paddle_vector.x };

        return Vector2Normalize(paddle_normal);
    }

    bool is_circle_inside(const Vector2 position, const float radius) {
        static Vector2 half_point{ FONT_SIZE / 2.f, FONT_SIZE / 2.f };
        if (!m_active) return false;
        if (CheckCollisionCircleLine(position,
                                     radius,
                                     m_begin + half_point,
                                     m_end + half_point)) return true;
        return false;
    }

    void draw() {
        static Vector2 half_point{ FONT_SIZE / 2.f, FONT_SIZE / 2.f };
        static int point_radius   = 5;
        static int line_thickness = 5;

        if (!m_active) return;

        DrawLineEx(m_begin + half_point, m_end + half_point, line_thickness, BAR_COLOUR);
        DrawCircleV(m_begin + half_point, point_radius,  BAR_COLOUR);
        DrawCircleV(m_end + half_point, point_radius,  BAR_COLOUR);
    }
private:
    Vector2 m_begin;
    Vector2 m_end;
    bool m_active;
};

#endif // PADDLE_HPP_
