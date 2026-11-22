# Building WARVA

This is the Sprint 1 walking skeleton: a GLFW/OpenGL 3.3 window, a fixed
1/60 s game loop, an isometric floor, click-to-move, and depth-sorted
placeholder sprites.

## Prerequisites

### Windows (target platform)

- Windows 10/11 x64
- Visual Studio 2022 Build Tools with **Desktop development with C++**
- CMake 3.21 or newer
- Git

### Linux (optional, for quick testing)

- A C++17 compiler
- CMake 3.21+
- X11 development headers (or Wayland)
- OpenGL development headers

GLFW and GLM are downloaded automatically on the first configure. The
versions are pinned in `CMakeLists.txt`. GLAD, `stb_image`, and
`stb_truetype` are vendored in `third_party/`, so the runtime build has no
external asset dependency.

## Windows build

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug
.\build\Debug\warva.exe
```

For the release build:

```powershell
cmake --build build --config Release
.\build\Release\warva.exe
```

## Linux build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
./build/warva
```

Run the logic tests with:

```bash
ctest --test-dir build --output-on-failure
```

## Controls

- Move the mouse to highlight an isometric tile.
- Left-click a free tile to walk the staff character there.
- Red highlight means the tile is occupied by a table or another character.
- The window title shows the current FPS.

## What this skeleton already demonstrates

- OpenGL 3.3 Core context and GLAD loading (`WV-003`).
- Fixed-timestep update with an accumulator, independent of render FPS.
- Isometric tile projection and inverse screen-to-tile conversion.
- Mouse hover, click, and hold tracking (`WV-005`).
- Sprite batch, alpha blending, `GL_NEAREST` filtering.
- Depth sorting by foot position with stable layer/id tie-breakers (`WV-007`).
- Letterboxed 1280x720 virtual resolution on window resize.
- Automatic pause when the window loses focus.
- A small logic test target for isometric math.

## Not implemented yet

- Embedded PNG/font asset pipeline (`WV-006`).
- Real character art (`WV-029`, `WV-030`).
- Customer, order, cooking, cashier, and parking logic (`S2`/`S3`).
- Pathfinding around obstacles; the player currently walks in a straight line.

## Source layout

| Path | Responsibility |
| --- | --- |
| `src/App.*` | Window, OpenGL context, main loop, shutdown. |
| `src/Input.*` | Mouse position, click, hold, virtual-screen mapping. |
| `src/World.*` | Grid, characters, tables, movement, hover state. |
| `src/Renderer.*` | Sprite batch, isometric floor, depth-sorted drawing. |
| `src/Shader.*` | Shader compile/link wrapper with error reporting. |
| `src/Texture.*` | RGBA texture creation and PNG loading via `stb_image`. |
| `src/Iso.h` | Isometric coordinate conversion. |
| `tests/test_iso.cpp` | Coordinate round-trip checks. |
