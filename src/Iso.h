#pragma once

#include <cmath>

namespace warva {

// Virtual resolution. The window is letterboxed to preserve this aspect ratio.
constexpr float kVirtualWidth = 1280.0f;
constexpr float kVirtualHeight = 720.0f;

// Isometric tile dimensions from the technical documentation.
constexpr float kTileWidth = 64.0f;
constexpr float kTileHeight = 32.0f;

// Screen-space anchor of tile (0, 0), in virtual pixels.
constexpr float kOriginX = 640.0f;
constexpr float kOriginY = 110.0f;

struct ScreenPoint {
    float x = 0.0f;
    float y = 0.0f;
};

// tileToScreen uses the formulas from 02-Technical-Documentation.md:
//   screenX = originX + (x - y) * tileWidth  / 2
//   screenY = originY + (x + y) * tileHeight / 2 - elevation
inline ScreenPoint tileToScreen(float tileX, float tileY, float elevation = 0.0f) {
    return {
        kOriginX + (tileX - tileY) * (kTileWidth * 0.5f),
        kOriginY + (tileX + tileY) * (kTileHeight * 0.5f) - elevation,
    };
}

// Inverse of tileToScreen for the ground plane (elevation = 0).
inline void screenToTile(float screenX, float screenY, int& tileX, int& tileY) {
    const float dx = (screenX - kOriginX) / (kTileWidth * 0.5f);
    const float dy = (screenY - kOriginY) / (kTileHeight * 0.5f);
    const float fx = (dx + dy) * 0.5f;
    const float fy = (dy - dx) * 0.5f;
    tileX = static_cast<int>(std::floor(fx));
    tileY = static_cast<int>(std::floor(fy));
}

inline bool insideVirtualScreen(float x, float y) {
    return x >= 0.0f && x < kVirtualWidth && y >= 0.0f && y < kVirtualHeight;
}

} // namespace warva
