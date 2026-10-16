#include <raylib.h>

#include "error.hpp"
#include "key.hpp"

Key::Key(char ch, Vector2 position) {
    m_char = ch;
    m_position = position;
    m_is_pressed = false;
}

void Key::activate(void) { m_is_pressed = true; }
void Key::deactivate(void) { m_is_pressed = false; }

char Key::get_char(void) { return m_char; }
Vector2 Key::get_position(void) { return m_position; }
bool Key::is_pressed(void) { return m_is_pressed; }

KeyManager::KeyManager(Paddle& paddle) {
    static std::string char_keys = QWERTY_KEYS;
    float x = 10;
    float y = (WINDOW_HEIGHT / 2.f) + FONT_SIZE;
    int i = 0;

    for (char ch : char_keys) {
        if (ch == '\n') {
            x = 10 + ((i == 0) ? 25 : 70); // lmao
            y += FONT_SIZE * 2.2;
            i++;
            continue;
        } else if (ch == '\0') {
            break;
        }

        m_keys.push_back(Key{ ch, { x, y } });
        x += FONT_SIZE + 17;
    }

    m_paddle = &paddle;
}

void KeyManager::load_font(Font& font) {
    m_font = font;
}

void KeyManager::unload() {
    m_keys.clear();
}

void KeyManager::draw(void) {
    for (Key& key : m_keys) {
        std::string str(1, key.get_char());
        DrawTextEx(m_font,
                   str.c_str(),
                   key.get_position(),
                   FONT_SIZE,
                   0,
                   (key.is_pressed()) ?  FONT_ACTIVE_COLOUR : FONT_UNACTIVE_COLOUR);
    }
}

// Ehh I'm not proud of this function but it works
void KeyManager::update(void) {
    static const char backspace = 8; // Backspace = 8 in ASCII
    if (!(GetKeyPressed() >= 'A' && GetKeyPressed() <= 'Z') && GetKeyPressed() != backspace) return;

    if (IsKeyPressed(KEY_BACKSPACE)) {
        pop_line();
        return;
    }

    for (Key& key : m_keys) {
        if (IsKeyPressed(KEYBOARD_RAYLIB[key.get_char() - 'A'])) {
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
                m_paddle->unactivate();
            }

            if (key1 != nullptr && key2 != nullptr) {
                m_paddle->move(key1->get_position(), key2->get_position());
                m_paddle->activate();
            }

            key.activate();
            SoundManager::get().play_sound(SFX_KEY_PRESSED);
        }
    }
}

void KeyManager::pop_line(void) {
    if (!m_paddle->is_active()) return;

    SoundManager::get().play_sound(SFX_PADDLE_BREAK);

    key1->deactivate();
    key2->deactivate();
    key1 = nullptr;
    key2 = nullptr;
    m_paddle->unactivate();
}
