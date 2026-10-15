#include <optional>
#include <raylib.h>
#include <raymath.h>
#include <string>

#include "sound.hpp"
#include "ball.hpp"
#include "key.hpp"
#include "paddle.hpp"
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

void load(void) {
    SoundManager& sm = SoundManager::get();

    sm.add_sound(file_key_pressed, sizeof(file_key_pressed), "key_pressed");
    sm.add_sound(file_hit, sizeof(file_hit), "hit");
    sm.add_sound(file_blockbreak, sizeof(file_blockbreak), "block_break");

    sm.set_volume(0.5f);
}

int main(void) {
    InitWindow(WINDOW_WIDTH,
               WINDOW_HEIGHT,
               "Keyout");
    SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetTargetFPS(60);

    InitAudioDevice();

    load();

    Paddle paddle{};
    KeyManager km{paddle};
    SoundManager& sm = SoundManager::get();
    Ball ball{};

    paddle.activate();
    paddle.move({ 10.f, 300.f }, { WINDOW_WIDTH - 50.f, 300.f });

    while (!WindowShouldClose()) {
        // Update
        km.update();
        ball.update();

        if (paddle.is_circle_inside(ball.get_position(), ball.get_radius())) {
            std::optional<Vector2> normal = paddle.get_normal();
            if (normal.has_value()) {
                ball.set_velocity(Vector2Reflect(ball.get_velocity(), normal.value()));
                km.pop_line();
                sm.play_sound(SFX_BALL_HIT);
            }
        }

        if (IsKeyPressed(KEY_SPACE)) sm.play_sound(SFX_KEY_PRESSED);

        // Render
        BeginDrawing(); {
            ClearBackground(BLACK);
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
