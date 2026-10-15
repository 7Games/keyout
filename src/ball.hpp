#ifndef BALL_HPP_
#define BALL_HPP_

#include <raylib.h>
#include <raymath.h>

#include "sound.hpp"
#include "misc.hpp"

class Ball {
public:
    Ball(Vector2 position, Vector2 velocity);

    void draw(void);
    void update(void);

    Vector2 get_position(void);
    Vector2 get_velocity(void);
    void set_velocity(Vector2 velocity);
    int get_radius(void);
private:
    Vector2 m_position;
    Vector2 m_velocity;

    int m_radius;

    Color m_colour;

    SoundManager& sm = SoundManager::get();
};

#endif // BALL_HPP_
