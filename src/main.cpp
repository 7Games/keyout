#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <optional>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <format>
#include <print>

#include "sound.hpp"
#include "ball.hpp"
#include "key.hpp"
#include "paddle.hpp"
#include "brick.hpp"
#include "misc.hpp"
#include "error.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Assets

// Un-needed but we'll keep it here
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

enum GameState {
    GSTATE_MENU = 0,
    GSTATE_GAME,
    GSTATE_GAMEOVER,
};

// Game global stuff (bad to do ts but idc)

GameState state;

Font font;

SoundManager& sm = SoundManager::get();
Paddle paddle{};
KeyManager km{paddle};
Ball ball{};

std::vector<Brick> bricks;
bool has_made_brick_in_last_second = false;

float reset_timer = SECONDS_BEFORE_FREE_BALL;

std::string last_chars = "  ";

int score = 0;
int lives = 3;

void load(void) {
    SoundManager& sm = SoundManager::get();

    sm.add_sound(file_key_pressed, sizeof(file_key_pressed), SFX_KEY_PRESSED);
    sm.add_sound(file_hit, sizeof(file_hit), SFX_BALL_HIT);
    sm.add_sound(file_paddle_break, sizeof(file_paddle_break), SFX_PADDLE_BREAK);
    sm.add_sound(file_brick_break, sizeof(file_brick_break), SFX_BRICK_BREAK);

    sm.set_volume(0.5f);

    state = GSTATE_MENU;

    font = LoadFontFromMemory(".ttf",
                                publicpixel_font,
                                sizeof(publicpixel_font),
                                FONT_SIZE,
                                nullptr,
                                250);
    if (!IsFontValid(font)) Error::crash("Couldn't load font");

    km.load_font(font);
}

void update_menu(void) {
    lives = 3;
    score = 0;
    reset_timer = SECONDS_BEFORE_FREE_BALL;
    has_made_brick_in_last_second = false;

    km.update();
    if (GetCharPressed() >= 'A' && GetCharPressed() <= 'Z') {
        state = GSTATE_GAME;
        last_chars.erase(last_chars.begin());
        last_chars.push_back(GetCharPressed());
    }

    static int title_font_size = 32;
    static int sub_title_font_size = 19;

    Vector2 title_size = MeasureTextEx(font, "Keyout", title_font_size, 4);
    Vector2 title_pos = Vector2{WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f} - (title_size / 2.f);
    title_pos.y -= 150;

    Vector2 sub_title_size = MeasureTextEx(font, "Press any key\nto start", sub_title_font_size, 4);
    Vector2 sub_title_pos = Vector2{WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f} - (sub_title_size / 2.f);
    sub_title_pos.y -= 50;

    BeginDrawing(); {
        ClearBackground(BLACK);

        DrawTextEx(font,
                   "Keyout",
                   title_pos,
                   title_font_size,
                   4,
                   RED);

        DrawTextEx(font,
                   "Press any key\n  to start",
                   sub_title_pos,
                   sub_title_font_size,
                   4,
                   GRAY);

        km.draw();
    } EndDrawing();
}

void update_gameover(void) {
    last_chars = "  ";

    lives = 3;
    score = 0;
    has_made_brick_in_last_second = false;

    const float delta = GetFrameTime();

    if (reset_timer > 0) reset_timer -= delta;
    if (reset_timer < 0) reset_timer = 0;

    if (reset_timer == 0 && (GetCharPressed() >= 'A' && GetCharPressed() <= 'Z')) {
        state = GSTATE_MENU;
    }

    km.pop_line();

    static int title_font_size = 32;
    static int sub_title_font_size = 19;

    Vector2 title_size = MeasureTextEx(font, "GAME OVER", title_font_size, 4);
    Vector2 title_pos = Vector2{WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f} - (title_size / 2.f);
    title_pos.y -= 20;

    Vector2 sub_title_size = MeasureTextEx(font, "Press any key\nto return to menu", sub_title_font_size, 4);
    Vector2 sub_title_pos = Vector2{WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f} - (sub_title_size / 2.f);
    sub_title_pos.y += 40;

    BeginDrawing(); {
        ClearBackground(BLACK);
        DrawTextEx(font,
                   "GAME OVER",
                   title_pos,
                   title_font_size,
                   4,
                   RED);
        DrawTextEx(font,
                   "  Press any key\nto return to menu",
                   sub_title_pos,
                   sub_title_font_size,
                   4,
                   GRAY);
    } EndDrawing();
}

void update_game(void) {
    // Update
    const int time = (int)GetTime();
    const float delta = GetFrameTime();

    char key_press = GetCharPressed();
    if (key_press >= 'a' && key_press <= 'z') {
        last_chars.erase(last_chars.begin());
        last_chars.push_back(key_press - 32);
    }

    if (lives == 0) {
        reset_timer = 0.5;
        state = GSTATE_GAMEOVER;
    }

    if (reset_timer > 0) reset_timer -= delta;
    if (reset_timer < 0) reset_timer = 0;

    km.update();
    if (reset_timer == 0) {
        if (ball.update()) {
            reset_timer = SECONDS_BEFORE_FREE_BALL;
            lives--;
        }
    }

    const Vector2 ball_pos = ball.get_position();
    const float ball_rad = ball.get_radius();
    const Vector2 ball_vel = ball.get_velocity();

    if (paddle.is_circle_inside(ball_pos, ball_rad)) {
        Vector2 paddle_normal = paddle.get_normal().value_or(ball_vel);
        ball.set_velocity(Vector2Reflect(ball_vel, paddle_normal));
        km.pop_line();
        sm.play_sound(SFX_BALL_HIT);
    }

    for (std::size_t i = 0; i < bricks.size(); i++) {
        if (bricks.at(i).is_circle_inside(ball_pos, ball_rad)) {
            Vector2 brick_normal = bricks[i].where_circle_hit(ball_pos, ball_rad);

            bricks.erase(bricks.begin() + i);
            sm.play_sound(SFX_BALL_HIT);
            sm.play_sound(SFX_BRICK_BREAK);

            score += 10;

            ball.set_velocity(Vector2Reflect(ball_vel, brick_normal));
        }
    }

    if (reset_timer == 0 && !has_made_brick_in_last_second && time % SECONDS_BETWEEN_BRICKS == 0) {
        float x = rand() % BRICKS_MAX_WIDTH;
        float y = rand() % BRICKS_MAX_HEIGHT;

    brick_pos_check:
        for (Brick& b : bricks) {
            Vector2 bpos = b.get_position();
            bpos.x /= (BRICK_WIDTH + 5);
            bpos.x -= BRICK_START_X;
            bpos.y /= (BRICK_HEIGHT + 5);
            bpos.y -= BRICK_START_Y;
            if (bpos == Vector2{ x, y }) {
                x = rand() % BRICKS_MAX_WIDTH;
                y = rand() % BRICKS_MAX_HEIGHT;
                goto brick_pos_check;
            }
        }

        has_made_brick_in_last_second = true;
        Brick b{{BRICK_START_X + x, BRICK_START_Y + y}};
        bricks.push_back(b);
    } if (has_made_brick_in_last_second && time % SECONDS_BETWEEN_BRICKS == 1) {
        has_made_brick_in_last_second = false;
    }

    Vector2 last_char_size = MeasureTextEx(font, last_chars.c_str(), 50, 5);

    // Render
    BeginDrawing(); {
        ClearBackground(BLACK);
        // Draw last chars
        DrawTextEx(font, last_chars.c_str(), Vector2{WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f} - last_char_size / 2.f, 50, 5, DARKGRAY);

        //

        for (Brick& b : bricks) {
            b.draw();
        }

        km.draw();
        ball.draw();
        paddle.draw();

        // Draw UI
        const float lives_size = MeasureText(std::format("Lives: {}  ", lives).c_str(), UI_FONT_SIZE);
        DrawText(std::format("Lives: {} ", lives).c_str(),
                 (WINDOW_WIDTH / 2.f) - lives_size,
                 WINDOW_HEIGHT - UI_FONT_SIZE - 5,
                 UI_FONT_SIZE,
                 WHITE);
        DrawText(std::format("| Score: {}", score).c_str(),
                 (WINDOW_WIDTH / 2.f),
                 WINDOW_HEIGHT - UI_FONT_SIZE - 5,
                 UI_FONT_SIZE,
                 WHITE);
    } EndDrawing();
}

int main(void) {
    InitWindow(WINDOW_WIDTH,
               WINDOW_HEIGHT,
               "Keyout");
    SetTargetFPS(60);
    InitAudioDevice();

    load();

    srand(time(0));

#ifdef __EMSCRIPTEN__
    while(true) {
#else
    while (!WindowShouldClose()) {
#endif
        switch (state) {
        case GSTATE_MENU:
            update_menu();
            break;
        case GSTATE_GAMEOVER:
            update_gameover();
            break;
        case GSTATE_GAME:
            update_game();
            break;
        }
    }

    sm.remove_all_sounds();
    km.unload();
    CloseWindow();
    CloseAudioDevice();

    return 0;
}
