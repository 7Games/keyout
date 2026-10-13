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
#define BALL_COLOUR WHITE

#define INITIAL_BALL_RADIUS 5
#define INITIAL_BALL_SPEED 2

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
void operator+=(Vector2& lhs, Vector2 const& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
}
Vector2 operator*(Vector2 const& lhs, float const& rhs) {
    return {
        lhs.x * rhs,
        lhs.y * rhs,
    };
}
Vector2 operator-(Vector2 const& rhs) {
    return {
        -rhs.x,
        -rhs.y,
    };
}

class Ball {
public:
    Ball(Vector2 position) {
        m_position = position;
        m_velocity = { 0, 0 };
        m_radius   = INITIAL_BALL_RADIUS;
        m_speed    = INITIAL_BALL_SPEED;
    }

    void draw(void) {
        DrawCircleV(m_position, m_radius, BALL_COLOUR);
    }

    void change_velocity(Vector2 velocity) {
        m_velocity = velocity;
    }

    void update(void) {
        if (m_position.y <= 0) {
            m_velocity.y = -m_velocity.y;
            m_position.y = 1;
        }
        if (m_position.x <= 0) {
            m_velocity.x = -m_velocity.x;
            m_position.x = 1;
        }
        if (m_position.x >= INITIAL_WINDOW_WIDTH) {
            m_velocity.x = -m_velocity.x;
            m_position.x = INITIAL_WINDOW_WIDTH - 1;
        }
        if (m_position.y > INITIAL_WINDOW_HEIGHT + m_radius) {
            CloseWindow();
        }

        m_position += (m_velocity * m_speed);
    }

    Vector2 get_position(void) { return m_position; }
    int get_radius(void) { return m_radius; }
    Vector2 get_velocity(void) { return m_velocity; }
private:
    Vector2 m_position;
    Vector2 m_velocity;
    int m_radius;
    float m_speed;
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
        float x = 32;
        float y = (INITIAL_WINDOW_HEIGHT / 2.f) + FONT_SIZE;
        int i = 0;

        for (char ch : char_keys) {
            if (ch == '\n') {
                i++;
                x = 32 + ((i == 1) ? 20 : 52); // lmao
                y += FONT_SIZE * 2;
                continue;
            } else if (ch == '\0') {
                break;
            }

            m_keys.push_back(Key{ ch, { x, y } });
            x += FONT_SIZE + 12;
        }
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
    Ball ball{{(INITIAL_WINDOW_WIDTH / 2.f) - (INITIAL_BALL_RADIUS / 2.f), 10}};
    ball.change_velocity({ 1, 1 });

    while (!WindowShouldClose()) {
        // Update
        km.update();
        ball.update();

        if (km.is_point_in_line(ball)) {
            ball.change_velocity(-ball.get_velocity());
            km.pop_line();
        }


        // Render
        BeginDrawing(); {
            ClearBackground(BLACK);
            km.draw(font);
            ball.draw();
        } EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();

    return 0;
}
