#ifndef MISC_HPP_
#define MISC_HPP_

#define WINDOW_WIDTH  490
#define WINDOW_HEIGHT 500

#define FONT_SIZE 32

#define FONT_ACTIVE_COLOUR   Color{ 220, 220, 220, 255 }
#define FONT_UNACTIVE_COLOUR DARKGRAY

#define BAR_COLOUR WHITE

#define INITIAL_BALL_POSITION {(WINDOW_WIDTH / 2.f) - (INITIAL_BALL_RADIUS / 2.f), 300}
#define INITIAL_BALL_VELOCITY { 2.5, 2.5 }
#define INITIAL_BALL_RADIUS   5
#define INITIAL_BALL_COLOUR   WHITE

#define BRICK_WIDTH  50
#define BRICK_HEIGHT 20
#define BRICK_COLOUR WHITE

#define BRICK_START_X 0.5f
#define BRICK_START_Y 1.f

#define BRICKS_MAX_HEIGHT 7
#define BRICKS_MAX_WIDTH  8

// Asset names

#define SFX_BALL_HIT "hit"
#define SFX_KEY_PRESSED "key_pressed"
#define SFX_PADDLE_BREAK "paddle_break"
#define SFX_BRICK_BREAK "brick_break"

#endif // MISC_HPP_
