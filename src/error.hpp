#ifndef ERROR_HPP_
#define ERROR_HPP_

#include <cstddef>
#include <cstdlib>
#include <string>
#include <raylib.h>

#include "misc.hpp"

#define ERROR_STRING_SIZE 24

namespace Error {
    inline void crash(std::string error) {
        std::size_t error_size = MeasureText(error.c_str(), ERROR_STRING_SIZE);

#ifdef __EMSCRIPTEN__
    while(true) {
#else
    while (!WindowShouldClose()) {
#endif
            BeginDrawing();

            ClearBackground(BLACK);

            DrawText(error.c_str(),
                     (WINDOW_WIDTH / 2.f) - (error_size / 2.f),
                     (WINDOW_HEIGHT / 2.f) - (ERROR_STRING_SIZE / 2.f),
                     ERROR_STRING_SIZE,
                     RED);

            EndDrawing();
        }

        exit(-1);
    }
}

#endif // ERROR_HPP_
