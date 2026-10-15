#ifndef KEY_HPP_
#define KEY_HPP_

#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>

#include "sound.hpp"
#include "ball.hpp"
#include "misc.hpp"

const unsigned char publicpixel_font[] = {
#embed "../res/publicpixel.ttf"
};

class Key {
public:
    Key(char ch, Vector2 position) {
        this->ch = ch;
        this->position = position;
        this->is_pressed = false;
    }

    void draw(Font const& font) {
        std::string str(1, this->ch);
        DrawTextEx(font,
                   str.c_str(),
                   this->position,
                   FONT_SIZE,
                   0,
                   (this->is_pressed) ?  FONT_ACTIVE_COLOUR : FONT_UNACTIVE_COLOUR);
    }

    void activate(void) {
        this->is_pressed = true;
    }
    void deactivate(void) {
        this->is_pressed = false;
    }

    bool is_pressed;
    char ch;
    Vector2 position;
};

class KeyManager {
public:
    KeyManager() {
        static std::string char_keys = "QWERTYUIOP\nASDFGHJKL\nZXCVBNM\0";
        float x = 10;
        float y = (WINDOW_HEIGHT / 2.f) + FONT_SIZE;
        int i = 0;

        for (char ch : char_keys) {
            if (ch == '\n') {
                x = 10 + ((i == 0) ? 25 : 70); // lmao
                y += FONT_SIZE * 2.5;
                i++;
                continue;
            } else if (ch == '\0') {
                break;
            }

            m_keys.push_back(Key{ ch, { x, y } });
            x += FONT_SIZE + 17;
        }

        m_font = LoadFontFromMemory(".ttf",
                                    publicpixel_font,
                                    sizeof(publicpixel_font),
                                    FONT_SIZE,
                                    nullptr,
                                    250);
        if (!IsFontValid(m_font)) exit(1);
    }

    void unload() {
        m_keys.clear();
        UnloadFont(m_font);
    }

    Vector2 get_normal_of_paddle() {
        if (key1 == nullptr || key2 == nullptr) return Vector2{ 0, 0 };
        Vector2 paddle_vector = key2->position - key1->position;
        Vector2 paddle_normal = { -paddle_vector.y, paddle_vector.x };
        return Vector2Normalize(paddle_normal);
    }

    void draw(void) {
        for (Key& key : m_keys) {
            key.draw(m_font);
        }
        draw_lines();
    }

    void update(void) {
        for (Key& key : m_keys) {
            if (IsKeyPressed(KEYBOARD_RAYLIB[key.ch - 'A'])) {
                if (key1 == nullptr) {
                    key1 = &key;
                }
                else if (key2 == nullptr) {
                    key2 = &key;
                    if (key1 == key2) {
                        key2 = nullptr;
                    }
                }
                else {
                    key1->deactivate();
                    key2->deactivate();
                    key1 = &key;
                    key2 = nullptr;
                }
                key.activate();
                SoundManager::get().play_sound("key_pressed");
            }
        }
    }

    bool is_point_in_line(Ball& ball) {
        static Vector2 half_point{ HALF_FONT_SIZE, HALF_FONT_SIZE };
        if (key1 == nullptr || key2 == nullptr) return false;
        if (CheckCollisionCircleLine(ball.get_position(),
                                     ball.get_radius(),
                                     key1->position + half_point,
                                     key2->position + half_point)) return true;
        return false;
    }

    void pop_line(void) {
        key1->deactivate();
        key2->deactivate();
        key1 = nullptr;
        key2 = nullptr;
    }
private:
    Font m_font;

    void draw_lines(void) {
        static Vector2 half_point{ HALF_FONT_SIZE, HALF_FONT_SIZE };
        static int point_radius = 5;
        static int line_thickness = 5;
        if (key1 != nullptr && key2 != nullptr) {
            DrawLineEx(key1->position + half_point, key2->position + half_point, line_thickness,  BAR_COLOUR);
            DrawCircleV(key1->position + half_point, point_radius,  BAR_COLOUR);
            DrawCircleV(key2->position + half_point, point_radius,  BAR_COLOUR);
        }
    }

    KeyboardKey KEYBOARD_RAYLIB[26] = {
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F,
        KEY_G,
        KEY_H,
        KEY_I,
        KEY_J,
        KEY_K,
        KEY_L,
        KEY_M,
        KEY_N,
        KEY_O,
        KEY_P,
        KEY_Q,
        KEY_R,
        KEY_S,
        KEY_T,
        KEY_U,
        KEY_V,
        KEY_W,
        KEY_X,
        KEY_Y,
        KEY_Z
    };

    std::vector<Key> m_keys;

    Key* key1 = nullptr;
    Key* key2 = nullptr;
};

#endif // KEY_HPP_
