# Third-party notices

WARVA uses the following third-party software. Versions are pinned in
`CMakeLists.txt` or vendored in `third_party/`. Keep this file updated when a
dependency or version changes.

| Dependency | Version | License | Use |
| --- | --- | --- | --- |
| GLFW | 3.4 | zlib/libpng | Window, OpenGL context, input. |
| GLM | 1.0.1 | MIT (also offered under the Happy Bunny license) | Vector and matrix math. |
| GLAD | Generated for OpenGL 3.3 Core | MIT | OpenGL function loader. |
| stb_image | Current vendored snapshot | Public domain / MIT | PNG loading. |
| stb_truetype | Current vendored snapshot | Public domain / MIT | Font atlas generation (planned). |
| Khronos `khrplatform.h` | Bundled with GLAD | Khronos MIT-style notice | Platform types for GLAD. |

GLFW and GLM are downloaded from their official release archives during the
first CMake configure. Their license files are included in the fetched source
trees. GLAD, `stb_image`, and `stb_truetype` are vendored in `third_party/`
and retain their original copyright notices.

Before distributing the game, verify that every art, audio, and font asset has
a recorded source and license, as required by `06-Character-Art-Direction.md`.
