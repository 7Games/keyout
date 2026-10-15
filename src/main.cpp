#include <cstdlib>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>

#include "sound.hpp"
#include "ball.hpp"
#include "key.hpp"
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

const unsigned char file_blockbreak[] = {
#embed "../res/blockbreak.ogg"
};

int main(void) {
    InitWindow(WINDOW_WIDTH,
               WINDOW_HEIGHT,
               "Keyout");
    SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetTargetFPS(60);

    InitAudioDevice();

    SoundManager& sm = SoundManager::get();

    KeyManager km{};
    Ball ball{{(WINDOW_WIDTH / 2.f) - (INITIAL_BALL_RADIUS / 2.f), 10}, INITIAL_BALL_VELOCITY};

    sm.add_sound(file_key_pressed, sizeof(file_key_pressed), "key_pressed");
    sm.add_sound(file_hit, sizeof(file_hit), "hit");
    sm.add_sound(file_blockbreak, sizeof(file_blockbreak), "block_break");

    sm.set_volume(0.5f);

    while (!WindowShouldClose()) {
        // Update
        km.update();
        ball.update();

        if (km.is_point_in_line(ball)) {
            Vector2 normal = km.get_normal_of_paddle();
            ball.set_velocity(Vector2Reflect(ball.get_velocity(), normal));
            km.pop_line();
            sm.play_sound("hit");
        }

        if (IsKeyPressed(KEY_SPACE)) sm.play_sound("key_pressed");

        // Render
        BeginDrawing(); {
            ClearBackground(BLACK);
            km.draw();
            ball.draw();
        } EndDrawing();
    }

    sm.remove_all_sounds();
    km.unload();
    CloseWindow();
    CloseAudioDevice();

    return 0;
}
