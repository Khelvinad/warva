#include "Renderer.h"

#include "Iso.h"

#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <vector>

namespace {

const char* kVertexShader = R"GLSL(
#version 330 core
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec4 aColor;

uniform mat4 uProjection;

out vec2 vUV;
out vec4 vColor;

void main() {
    gl_Position = uProjection * vec4(aPosition, 0.0, 1.0);
    vUV = aUV;
    vColor = aColor;
}
)GLSL";

const char* kFragmentShader = R"GLSL(
#version 330 core
in vec2 vUV;
in vec4 vColor;

uniform sampler2D uTexture;

out vec4 FragColor;

void main() {
    FragColor = texture(uTexture, vUV) * vColor;
}
)GLSL";

} // namespace

namespace warva {

Renderer::~Renderer() {
    shutdown();
}

bool Renderer::init(std::string& error) {
    if (!shader_.build(kVertexShader, kFragmentShader, error)) return false;

    projection_ = glm::ortho(0.0f, kVirtualWidth, kVirtualHeight, 0.0f, -1.0f, 1.0f);
    projectionLocation_ = shader_.uniformLocation("uProjection");
    textureLocation_ = shader_.uniformLocation("uTexture");
    if (projectionLocation_ < 0 || textureLocation_ < 0) {
        error = "shader is missing uProjection or uTexture";
        return false;
    }

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, uv)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, color)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (!createPlaceholderAtlas(error)) return false;

    vertices_.reserve(8192);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}

void Renderer::shutdown() {
    if (vbo_ != 0) {
        glDeleteBuffers(1, &vbo_);
        vbo_ = 0;
    }
    if (vao_ != 0) {
        glDeleteVertexArrays(1, &vao_);
        vao_ = 0;
    }
    atlas_.destroy();
    shader_.destroy();
}

bool Renderer::createPlaceholderAtlas(std::string& error) {
    constexpr int kAtlasWidth = 128;
    constexpr int kAtlasHeight = 64;

    std::vector<unsigned char> pixels(
        static_cast<std::size_t>(kAtlasWidth) * kAtlasHeight * 4, 0);

    auto put = [&](int x, int y, int r, int g, int b, int a = 255) {
        if (x < 0 || y < 0 || x >= kAtlasWidth || y >= kAtlasHeight) return;
        const std::size_t index =
            (static_cast<std::size_t>(y) * kAtlasWidth + x) * 4;
        pixels[index + 0] = static_cast<unsigned char>(r);
        pixels[index + 1] = static_cast<unsigned char>(g);
        pixels[index + 2] = static_cast<unsigned char>(b);
        pixels[index + 3] = static_cast<unsigned char>(a);
    };

    auto rect = [&](int x0, int y0, int x1, int y1, int r, int g, int b,
                    int a = 255) {
        for (int y = y0; y < y1; ++y) {
            for (int x = x0; x < x1; ++x) {
                put(x, y, r, g, b, a);
            }
        }
    };

    // Solid white block used by colored quads (floor, shadows, tables).
    rect(0, 0, 8, 8, 255, 255, 255, 255);

    struct Palette {
        int hair[3];
        int skin[3];
        int shirt[3];
        int lower[3];
        int belt[3];
        int pants[3];
        int shoes[3];
        int accent[3];
    };

    const Palette palettes[3] = {
        {{45, 32, 25}, {232, 190, 150}, {238, 228, 205}, {46, 92, 62},
         {165, 62, 45}, {55, 45, 40}, {35, 30, 28}, {120, 45, 35}},
        {{30, 25, 22}, {225, 180, 140}, {215, 215, 215}, {52, 92, 150},
         {30, 40, 60}, {40, 45, 55}, {25, 25, 30}, {35, 60, 100}},
        {{35, 28, 24}, {210, 165, 125}, {150, 112, 72}, {110, 80, 55},
         {70, 50, 35}, {60, 50, 40}, {30, 25, 22}, {90, 65, 45}},
    };

    auto drawCharacter = [&](int ox, int oy, int variant, const Palette& p) {
        // Hair and face.
        rect(ox + 4, oy + 1, ox + 12, oy + 5, p.hair[0], p.hair[1], p.hair[2]);
        rect(ox + 4, oy + 4, ox + 5, oy + 8, p.hair[0], p.hair[1], p.hair[2]);
        rect(ox + 11, oy + 4, ox + 12, oy + 8, p.hair[0], p.hair[1], p.hair[2]);
        rect(ox + 5, oy + 4, ox + 11, oy + 10, p.skin[0], p.skin[1], p.skin[2]);
        put(ox + 6, oy + 6, 30, 25, 22);
        put(ox + 9, oy + 6, 30, 25, 22);

        // Torso and arms.
        rect(ox + 4, oy + 10, ox + 12, oy + 15, p.shirt[0], p.shirt[1], p.shirt[2]);
        rect(ox + 2, oy + 11, ox + 4, oy + 17, p.shirt[0], p.shirt[1], p.shirt[2]);
        rect(ox + 12, oy + 11, ox + 14, oy + 17, p.shirt[0], p.shirt[1], p.shirt[2]);
        rect(ox + 2, oy + 17, ox + 4, oy + 18, p.skin[0], p.skin[1], p.skin[2]);
        rect(ox + 12, oy + 17, ox + 14, oy + 18, p.skin[0], p.skin[1], p.skin[2]);

        // Lower body, belt, legs, shoes.
        rect(ox + 5, oy + 13, ox + 11, oy + 21, p.lower[0], p.lower[1], p.lower[2]);
        rect(ox + 4, oy + 13, ox + 12, oy + 14, p.belt[0], p.belt[1], p.belt[2]);
        rect(ox + 5, oy + 21, ox + 7, oy + 23, p.pants[0], p.pants[1], p.pants[2]);
        rect(ox + 9, oy + 21, ox + 11, oy + 23, p.pants[0], p.pants[1], p.pants[2]);
        rect(ox + 4, oy + 23, ox + 7, oy + 24, p.shoes[0], p.shoes[1], p.shoes[2]);
        rect(ox + 9, oy + 23, ox + 12, oy + 24, p.shoes[0], p.shoes[1], p.shoes[2]);

        if (variant == 1) {
            // Satchel.
            rect(ox + 12, oy + 14, ox + 15, oy + 19,
                 p.accent[0], p.accent[1], p.accent[2]);
        } else if (variant == 2) {
            // Small briefcase.
            rect(ox + 12, oy + 18, ox + 15, oy + 21,
                 p.accent[0], p.accent[1], p.accent[2]);
        }
    };

    const int offsets[3][2] = {{16, 8}, {48, 8}, {80, 8}};
    for (int i = 0; i < 3; ++i) {
        drawCharacter(offsets[i][0], offsets[i][1], i, palettes[i]);
        characterUV_[i].topLeft = {
            (static_cast<float>(offsets[i][0]) + 0.5f) / kAtlasWidth,
            (static_cast<float>(offsets[i][1]) + 0.5f) / kAtlasHeight,
        };
        characterUV_[i].bottomRight = {
            (static_cast<float>(offsets[i][0]) + 15.5f) / kAtlasWidth,
            (static_cast<float>(offsets[i][1]) + 23.5f) / kAtlasHeight,
        };
    }

    whiteUV_ = {4.5f / kAtlasWidth, 4.5f / kAtlasHeight};
    return atlas_.createRGBA(kAtlasWidth, kAtlasHeight, pixels.data(), error);
}

void Renderer::render(const World& world, int framebufferWidth, int framebufferHeight) {
    beginFrame(framebufferWidth, framebufferHeight);
    vertices_.clear();
    drawFloor(world);
    drawHover(world);
    drawShadows(world);
    drawEntities(world);
    flush();
}

void Renderer::beginFrame(int framebufferWidth, int framebufferHeight) {
    if (framebufferWidth <= 0 || framebufferHeight <= 0) return;

    const float targetAspect = kVirtualWidth / kVirtualHeight;
    int viewWidth = 0;
    int viewHeight = 0;
    int offsetX = 0;
    int offsetY = 0;

    if (static_cast<float>(framebufferWidth) / framebufferHeight > targetAspect) {
        viewHeight = framebufferHeight;
        viewWidth = static_cast<int>(static_cast<float>(viewHeight) * targetAspect);
        offsetX = (framebufferWidth - viewWidth) / 2;
    } else {
        viewWidth = framebufferWidth;
        viewHeight = static_cast<int>(static_cast<float>(viewWidth) / targetAspect);
        offsetY = (framebufferHeight - viewHeight) / 2;
    }

    glViewport(offsetX, offsetY, viewWidth, viewHeight);
    glClearColor(0.09f, 0.075f, 0.065f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawFloor(const World& world) {
    for (int y = 0; y < world.gridHeight(); ++y) {
        for (int x = 0; x < world.gridWidth(); ++x) {
            const ScreenPoint center =
                tileToScreen(static_cast<float>(x), static_cast<float>(y));
            const bool alternate = ((x + y) & 1) == 0;
            const glm::vec4 color = alternate
                ? glm::vec4(0.42f, 0.35f, 0.27f, 1.0f)
                : glm::vec4(0.37f, 0.31f, 0.24f, 1.0f);
            pushDiamond(center.x, center.y, kTileWidth * 0.5f - 1.0f,
                        kTileHeight * 0.5f - 1.0f, color);
        }
    }
}

void Renderer::drawHover(const World& world) {
    if (!world.hoverValid()) return;

    const ScreenPoint center = tileToScreen(
        static_cast<float>(world.hoverX()), static_cast<float>(world.hoverY()));
    const glm::vec4 color = world.hoverBlocked()
        ? glm::vec4(0.85f, 0.25f, 0.20f, 0.38f)
        : glm::vec4(0.95f, 0.80f, 0.30f, 0.30f);
    pushDiamond(center.x, center.y, kTileWidth * 0.5f - 2.0f,
                kTileHeight * 0.5f - 1.0f, color);
}

void Renderer::drawShadows(const World& world) {
    for (const Character& character : world.characters()) {
        const ScreenPoint foot =
            tileToScreen(character.position.x, character.position.y);
        pushDiamond(foot.x, foot.y + 2.0f, 19.0f, 8.5f,
                    glm::vec4(0.0f, 0.0f, 0.0f, 0.28f));
        if (character.player) {
            pushDiamond(foot.x, foot.y + 2.0f, 24.0f, 11.0f,
                        glm::vec4(0.95f, 0.78f, 0.30f, 0.22f));
        }
    }
    for (const Table& table : world.tables()) {
        const ScreenPoint foot = tileToScreen(table.position.x, table.position.y);
        pushDiamond(foot.x, foot.y + 2.0f, 28.0f, 12.0f,
                    glm::vec4(0.0f, 0.0f, 0.0f, 0.30f));
    }
}

void Renderer::drawEntities(const World& world) {
    for (const Renderable& item : world.renderables()) {
        const ScreenPoint foot = tileToScreen(item.foot.x, item.foot.y);
        if (item.kind == Renderable::Kind::Character) {
            const CharacterUV& uv = characterUV_[item.variant % 3];
            pushQuad(foot.x, foot.y, 48.0f, 64.0f, glm::vec4(1.0f),
                     uv.topLeft, uv.bottomRight);
        } else {
            // Table body, then a darker and a lighter top diamond.
            pushQuad(foot.x, foot.y, 52.0f, 30.0f,
                     glm::vec4(0.45f, 0.30f, 0.18f, 1.0f), whiteUV_, whiteUV_);
            pushDiamond(foot.x, foot.y - 30.0f, 31.0f, 15.0f,
                        glm::vec4(0.32f, 0.21f, 0.13f, 1.0f));
            pushDiamond(foot.x, foot.y - 30.0f, 28.0f, 13.0f,
                        glm::vec4(0.60f, 0.42f, 0.25f, 1.0f));
        }
    }
}

void Renderer::pushQuad(float centerX, float bottomY, float width, float height,
                        const glm::vec4& color, const glm::vec2& uv0,
                        const glm::vec2& uv1) {
    const float left = centerX - width * 0.5f;
    const float right = centerX + width * 0.5f;
    const float top = bottomY - height;
    const float bottom = bottomY;

    vertices_.push_back({{left, top}, {uv0.x, uv0.y}, color});
    vertices_.push_back({{right, top}, {uv1.x, uv0.y}, color});
    vertices_.push_back({{right, bottom}, {uv1.x, uv1.y}, color});

    vertices_.push_back({{left, top}, {uv0.x, uv0.y}, color});
    vertices_.push_back({{right, bottom}, {uv1.x, uv1.y}, color});
    vertices_.push_back({{left, bottom}, {uv0.x, uv1.y}, color});
}

void Renderer::pushDiamond(float centerX, float centerY, float halfWidth,
                           float halfHeight, const glm::vec4& color) {
    const glm::vec2 uv = whiteUV_;
    const glm::vec2 top{centerX, centerY - halfHeight};
    const glm::vec2 right{centerX + halfWidth, centerY};
    const glm::vec2 bottom{centerX, centerY + halfHeight};
    const glm::vec2 left{centerX - halfWidth, centerY};

    vertices_.push_back({top, uv, color});
    vertices_.push_back({right, uv, color});
    vertices_.push_back({bottom, uv, color});

    vertices_.push_back({top, uv, color});
    vertices_.push_back({bottom, uv, color});
    vertices_.push_back({left, uv, color});
}

void Renderer::flush() {
    if (vertices_.empty() || vao_ == 0) return;

    shader_.use();
    glUniformMatrix4fv(projectionLocation_, 1, GL_FALSE, glm::value_ptr(projection_));
    atlas_.bind(GL_TEXTURE0);
    glUniform1i(textureLocation_, 0);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(vertices_.size() * sizeof(Vertex)),
                 vertices_.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_.size()));
    glBindVertexArray(0);
}

} // namespace warva
