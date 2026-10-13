#include <cstdlib>
#include <print>
#include <raylib.h>
#include <string>
#include <vector>

#define INITIAL_WINDOW_WIDTH  490
#define INITIAL_WINDOW_HEIGHT 500

#define FONT_SIZE 32
#define HALF_FONT_SIZE (FONT_SIZE / 2)

#define FONT_ACTIVE_COLOUR Color{ 220, 220, 220, 255 }
#define FONT_UNACTIVE_COLOUR DARKGRAY
#define BAR_COLOUR WHITE

const unsigned char publicpixel_font[] = {
#embed "publicpixel.ttf"
};
const unsigned char credits[] = {
#embed "credits.csv"
};

Vector2 operator+(Vector2 const& lhs, Vector2 const& rhs) {
    return {
        lhs.x + rhs.x,
        lhs.y + rhs.y,
    };
}

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
        float x = 32;
        float y = (INITIAL_WINDOW_HEIGHT / 2.f) + FONT_SIZE;

        // Yes we doing it manually lmao
        // Row 1
        m_keys.push_back(Key{ 'Q', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'W', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'E', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'R', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'T', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'Y', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'U', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'I', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'O', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'P', { x, y } });
        // Row 2
        y += FONT_SIZE * 2;
        x = 48;
        m_keys.push_back(Key{ 'A', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'S', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'D', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'F', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'G', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'H', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'J', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'K', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'L', { x, y } });
        x += FONT_SIZE + 12;
        // Row 3
        y += FONT_SIZE * 2;
        x = 92;
        m_keys.push_back(Key{ 'Z', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'X', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'C', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'V', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'B', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'N', { x, y } });
        x += FONT_SIZE + 12;
        m_keys.push_back(Key{ 'M', { x, y } });
    }

    void draw(Font const& font) {
        for (Key& key : m_keys) {
            key.draw(font);
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
            }
        }
    }
private:
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

// #define KEYBOARD "ZXCVBNM\nASDFGHJKL\nQWERTYUIOP\0"

int main(void) {
    InitWindow(INITIAL_WINDOW_WIDTH,
               INITIAL_WINDOW_HEIGHT,
               "Keyout");
    SetWindowMinSize(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
    SetTargetFPS(60);

    Font font = LoadFontFromMemory(".ttf",
                                   publicpixel_font,
                                   sizeof(publicpixel_font),
                                   FONT_SIZE,
                                   nullptr,
                                   250);
    if (!IsFontValid(font)) return 1;

    KeyManager km{};

    while (!WindowShouldClose()) {
        // Update

        km.update();

        // Render
        BeginDrawing(); {
            ClearBackground(BLACK);

            km.draw(font);
        } EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();

    return 0;
}
