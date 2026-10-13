# keyout

webasm build:

```sh
emcc -I./lib/raylib-6.0_webassembly/include/ ./lib/raylib-6.0_webassembly/lib/libraylib.web.a -s USE_GLFW=3 -s ASYNCIFY -DPLATFORM_WEB --shell-file ./lib/raylib-6.0_webassembly/minshell.html main.cpp -o index.html
```
