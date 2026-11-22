#include "Iso.h"

#include <cmath>
#include <cstdio>

namespace {

int failures = 0;

void expectNear(float actual, float expected, const char* label) {
    constexpr float kTolerance = 0.001f;
    if (std::fabs(actual - expected) > kTolerance) {
        std::fprintf(stderr, "FAIL %s: expected %.3f, got %.3f\n", label, expected, actual);
        ++failures;
    }
}

void expectTile(int tileX, int tileY, int expectedX, int expectedY) {
    const warva::ScreenPoint screen = warva::tileToScreen(
        static_cast<float>(tileX), static_cast<float>(tileY));
    int actualX = -1;
    int actualY = -1;
    warva::screenToTile(screen.x, screen.y, actualX, actualY);
    if (actualX != expectedX || actualY != expectedY) {
        std::fprintf(stderr, "FAIL round-trip (%d,%d): got (%d,%d)\n",
                     tileX, tileY, actualX, actualY);
        ++failures;
    }
}

} // namespace

int main() {
    using namespace warva;

    const ScreenPoint origin = tileToScreen(0.0f, 0.0f);
    expectNear(origin.x, kOriginX, "origin.x");
    expectNear(origin.y, kOriginY, "origin.y");

    const ScreenPoint right = tileToScreen(1.0f, 0.0f);
    expectNear(right.x, kOriginX + kTileWidth * 0.5f, "east.x");
    expectNear(right.y, kOriginY + kTileHeight * 0.5f, "east.y");

    const ScreenPoint down = tileToScreen(0.0f, 1.0f);
    expectNear(down.x, kOriginX - kTileWidth * 0.5f, "south.x");
    expectNear(down.y, kOriginY + kTileHeight * 0.5f, "south.y");

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            expectTile(x, y, x, y);
        }
    }

    if (failures == 0) {
        std::printf("iso_math: all checks passed\n");
        return 0;
    }
    std::fprintf(stderr, "iso_math: %d failure(s)\n", failures);
    return 1;
}
