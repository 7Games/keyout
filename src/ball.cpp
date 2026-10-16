#include "ball.hpp"

Ball::Ball(Vector2 position, Vector2 velocity) {
    m_position = position;
    m_velocity = velocity;
    m_radius   = INITIAL_BALL_RADIUS;
    m_colour   = INITIAL_BALL_COLOUR;
}

void Ball::draw(void) {
    DrawCircleV(m_position, m_radius, m_colour);
}

bool Ball::update(void) {
    if (m_position.y <= 0) {
        m_velocity.y = -m_velocity.y;
        m_position.y = 1;
        sm.play_sound(SFX_BALL_HIT);
    }
    if (m_position.x <= 0) {
        m_velocity.x = -m_velocity.x;
        m_position.x = 1;
        sm.play_sound(SFX_BALL_HIT);
    }
    if (m_position.x >= WINDOW_WIDTH) {
        m_velocity.x = -m_velocity.x;
        m_position.x = WINDOW_WIDTH - 1;
        sm.play_sound(SFX_BALL_HIT);
    }
    if (m_position.y > WINDOW_HEIGHT + m_radius) {
        m_position = INITIAL_BALL_POSITION;
        m_velocity = INITIAL_BALL_VELOCITY;
        return true;
    }

    m_position += m_velocity;
    return false;
}

// Getters and setters

Vector2 Ball::get_position(void) {
    return m_position;
}

Vector2 Ball::get_velocity(void) {
    return m_velocity;
}
void Ball::set_velocity(Vector2 velocity) {
    m_velocity = velocity;
}

int Ball::get_radius(void) {
    return m_radius;
}
