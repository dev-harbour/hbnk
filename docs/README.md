# Harbour - Nuklear - Simple DirectMedia Layer

Example of building a library for hbnksdl2.

## Installing on Windows

- MSYS2

Before installing the packages, make sure you're running the MinGW 64-bit shell.

   ```
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-SDL2
   git clone https://github.com/dev-harbour/hbnk
   ```
- To build the static library, execute:

   ```
   hbmk2 hbnksdl2.hbp
   ```

- To build and run samples and test code, navigate to the tests directory and execute:

   ```
   cd demo/sdl_renderer
   hbmk2 main.prg
   ```

## Installing on Linux

- Debian

   ```
   sudo apt update
   sudo apt install libsdl2-dev
   git clone https://github.com/dev-harbour/hbnk
   ```
- To build the static library, execute:

   ```
   hbmk2 hbnksdl2.hbp
   ```

- To build and run samples and test code, navigate to the examples directory and execute:

   ```
   cd demo/sdl_renderer
   hbmk2 main.prg
   ```
---
