#ifndef KEY_HPP_
#define KEY_HPP_

#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>

#include "sound.hpp"
#include "ball.hpp"
#include "paddle.hpp"
#include "misc.hpp"

const unsigned char publicpixel_font[] = {
#embed "../res/publicpixel.ttf"
};

class Key {
public:
    Key(char ch, Vector2 position) {
        m_char = ch;
        m_position = position;
        m_is_pressed = false;
    }

    void draw(Font const& font) {
        std::string str(1, m_char);
        DrawTextEx(font,
                   str.c_str(),
                   m_position,
                   FONT_SIZE,
                   0,
                   (m_is_pressed) ?  FONT_ACTIVE_COLOUR : FONT_UNACTIVE_COLOUR);
    }

    void activate(void) {
        m_is_pressed = true;
    }
    void deactivate(void) {
        m_is_pressed = false;
    }

    char get_char(void) { return m_char; }
private:
    bool m_is_pressed;
    char m_char;
    Vector2 m_position;
};

class KeyManager {
public:
    KeyManager(Paddle& paddle) {
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

        m_paddle = &paddle;
    }

    void unload() {
        m_keys.clear();
        UnloadFont(m_font);
    }

    void draw(void) {
        for (Key& key : m_keys) {
            key.draw(m_font);
        }
    }

    void update(void) {
        // for (Key& key : m_keys) {
            // if (IsKeyPressed(KEYBOARD_RAYLIB[key.ch - 'A'])) {
            //     if (key1 == nullptr) {
            //         key1 = &key;
            //     }
            //     else if (key2 == nullptr) {
            //         key2 = &key;
            //         if (key1 == key2) {
            //             key2 = nullptr;
            //         }
            //     }
            //     else {
            //         key1->deactivate();
            //         key2->deactivate();
            //         key1 = &key;
            //         key2 = nullptr;
            //         m_paddle->unactivate();
            //     }

            //     if (key1 != nullptr && key2 != nullptr) {
            //         m_paddle->move(key1->position, key2->position);
            //         m_paddle->activate();
            //     }

            //     key.activate();
            //     SoundManager::get().play_sound(SFX_KEY_PRESSED);
            // }
        // }
    }

    void pop_line(void) {
        if (key1 == nullptr && key2 == nullptr) return;
        key1->deactivate();
        key2->deactivate();
        key1 = nullptr;
        key2 = nullptr;
        m_paddle->unactivate();
    }
private:
    Font m_font;

    Paddle* m_paddle;

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
