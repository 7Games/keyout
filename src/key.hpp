#ifndef KEY_HPP_
#define KEY_HPP_

#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "sound.hpp"
#include "paddle.hpp"

const unsigned char publicpixel_font[] = {
#embed "../res/publicpixel.ttf"
};

#define QWERTY_KEYS "QWERTYUIOP\nASDFGHJKL\nZXCVBNM\0"

class Key {
public:
    Key(char ch, Vector2 position);

    void activate(void);
    void deactivate(void);

    char get_char(void);
    Vector2 get_position(void);
    bool is_pressed(void);
private:
    bool m_is_pressed;
    char m_char;
    Vector2 m_position;
};

class KeyManager {
public:
    KeyManager(Paddle& paddle);

    void load_font(Font& font);

    void draw(void);
    void update(void);

    void pop_line(void);
    void unload();
private:
    Font m_font;
    Paddle* m_paddle;
    std::vector<Key> m_keys;

    // Holy mid but what else can I do
    KeyboardKey KEYBOARD_RAYLIB[26] = {
        KEY_A, KEY_B, KEY_C, KEY_D,
        KEY_E, KEY_F, KEY_G, KEY_H,
        KEY_I, KEY_J, KEY_K, KEY_L,
        KEY_M, KEY_N, KEY_O, KEY_P,
        KEY_Q, KEY_R, KEY_S, KEY_T,
        KEY_U, KEY_V, KEY_W, KEY_X,
        KEY_Y, KEY_Z
    };

    Key* key1 = nullptr;
    Key* key2 = nullptr;
};

#endif // KEY_HPP_
