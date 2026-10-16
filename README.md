# Keyout

![Made in Cplusplus](https://img.shields.io/badge/C++-blue?logo=cplusplus&logoColor=white)
![Made with Raylib](https://img.shields.io/badge/Raylib-white?logo=raylib&logoColor=black)
![Licenced under Unlicense](https://img.shields.io/badge/License-Unlicense-black)
![Handmade](https://badges.ws/handmade)

<img alt="Cover" src="/res/cover.png" width="300px"/>

<img alt="Screenshot" src="/res/screenshot-1.png" width="300px"/>
<img alt="Screenshot" src="/res/screenshot-2.png" width="300px"/>

A remake of a prototype I saw on Twitter. All credits to [Connor Aitken](https://x.com/Aitken_/status/1555922340357328903) for the idea.

## How to build

Regular build (only tested on GNU/Linux):

```sh
make run
```

WebASM build:

```sh
emcc -I./lib/raylib-6.0_webassembly/include/ ./lib/raylib-6.0_webassembly/lib/libraylib.web.a -s USE_GLFW=3 -s ASYNCIFY -DPLATFORM_WEB --shell-file ./lib/raylib-6.0_webassembly/minshell.html main.cpp -o index.html
```

## Credits

 - Original idea by [Connor Aitken](https://x.com/Aitken_/status/1555922340357328903)
 - Game by [svn](https://svngms.neocities.org/)
 - Public Pixel Font created by [GGBotNet](https://ggbot.itch.io/public-pixel-font)
 - SFX by [Kenny](https://kenney.nl/assets/interface-sounds)
 - Written in C++ with [Raylib](https://www.raylib.com/)

## License

This code is licenced under the [UNLICENSE](https://unlicense.org/). All assets source or created by myself are licenced under [CC0](https://creativecommons.org/publicdomain/zero/1.0/).
