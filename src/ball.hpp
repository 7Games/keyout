#ifndef BALL_HPP_
#define BALL_HPP_

#include <raylib.h>
#include <raymath.h>

#include "sound.hpp"
#include "misc.hpp"

class Ball {
public:
    Ball(Vector2 position = INITIAL_BALL_POSITION,
         Vector2 velocity = INITIAL_BALL_VELOCITY);

    void draw(void);
    void update(void);

    // Only stuff we need really
    Vector2 get_position(void);
    Vector2 get_velocity(void);
    void set_velocity(Vector2 velocity);
    int get_radius(void);
private:
    Vector2 m_position;
    Vector2 m_velocity;

    int m_radius;

    Color m_colour;

    // tbf we could just grab it every time we need
    // it but it's better to just grab it once
    SoundManager& sm = SoundManager::get();
};

#endif // BALL_HPP_
