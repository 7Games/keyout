#include <cstddef>
#include <optional>
#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "sound.hpp"
#include "ball.hpp"
#include "key.hpp"
#include "paddle.hpp"
#include "brick.hpp"
#include "misc.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Assets

const unsigned char credits[] = {
#embed "../res/credits.csv"
};

// Audio

const unsigned char file_key_pressed[] = {
#embed "../res/keypressed.ogg"
};

const unsigned char file_hit[] = {
#embed "../res/hit.ogg"
};

const unsigned char file_brick_break[] = {
#embed "../res/brickbreak.ogg"
};

const unsigned char file_paddle_break[] = {
#embed "../res/paddlebreak.ogg"
};

void load(void) {
    SoundManager& sm = SoundManager::get();

    sm.add_sound(file_key_pressed, sizeof(file_key_pressed), SFX_KEY_PRESSED);
    sm.add_sound(file_hit, sizeof(file_hit), SFX_BALL_HIT);
    sm.add_sound(file_paddle_break, sizeof(file_paddle_break), SFX_PADDLE_BREAK);
    sm.add_sound(file_brick_break, sizeof(file_brick_break), SFX_BRICK_BREAK);

    sm.set_volume(0.5f);
}

int main(void) {
    InitWindow(WINDOW_WIDTH,
               WINDOW_HEIGHT,
               "Keyout");
    SetTargetFPS(60);
    InitAudioDevice();

    load();

    Paddle paddle{};
    KeyManager km{paddle};
    SoundManager& sm = SoundManager::get();
    Ball ball{};

    std::vector<Brick> bricks;

    float x = 0;
    float y = 0;
    for (int i = 0; i < BRICKS_MAX_HEIGHT; i++) {
        x = 0;
        for (int j = 0; j < BRICKS_MAX_WIDTH; j++) {
            Brick b{{BRICK_START_X + x, BRICK_START_Y + y}};
            x++;
            bricks.push_back(b);
        }
        y++;
    }

#ifdef __EMSCRIPTEN__
    while(true) {
#else
    while (!WindowShouldClose()) {
#endif
        // Update
        km.update();
        ball.update();

        if (paddle.is_circle_inside(ball.get_position(), ball.get_radius())) {
            Vector2 normal = paddle.get_normal().value_or(ball.get_velocity());
            ball.set_velocity(Vector2Reflect(ball.get_velocity(), normal));
            km.pop_line();
            sm.play_sound(SFX_BALL_HIT);
        }

        for (std::size_t i = 0; i < bricks.size(); i++) {
            if (bricks.at(i).is_circle_inside(ball.get_position(), ball.get_radius())) {
                bricks.erase(bricks.begin() + i);
                ball.set_velocity(Vector2Negate(ball.get_velocity()));
                sm.play_sound(SFX_BALL_HIT);
                sm.play_sound(SFX_BRICK_BREAK);
            }
        }

        // Render
        BeginDrawing(); {
            ClearBackground(BLACK);

            for (Brick& b : bricks) {
                b.draw();
            }

            km.draw();
            ball.draw();
            paddle.draw();
        } EndDrawing();
    }

    sm.remove_all_sounds();
    km.unload();
    CloseWindow();
    CloseAudioDevice();

    return 0;
}
