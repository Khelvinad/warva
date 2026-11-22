#pragma once

#include "Shader.h"
#include "Texture.h"
#include "World.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace warva {

class Renderer {
public:
    Renderer() = default;
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    bool init(std::string& error);
    void shutdown();

    void render(const World& world, int framebufferWidth, int framebufferHeight);

private:
    struct Vertex {
        glm::vec2 position;
        glm::vec2 uv;
        glm::vec4 color;
    };

    struct CharacterUV {
        glm::vec2 topLeft;
        glm::vec2 bottomRight;
    };

    bool createPlaceholderAtlas(std::string& error);

    void beginFrame(int framebufferWidth, int framebufferHeight);
    void drawFloor(const World& world);
    void drawHover(const World& world);
    void drawShadows(const World& world);
    void drawEntities(const World& world);
    void pushQuad(float centerX, float bottomY, float width, float height,
                  const glm::vec4& color, const glm::vec2& uv0, const glm::vec2& uv1);
    void pushDiamond(float centerX, float centerY, float halfWidth, float halfHeight,
                     const glm::vec4& color);
    void flush();

    Shader shader_;
    Texture atlas_;
    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    GLint projectionLocation_ = -1;
    GLint textureLocation_ = -1;
    glm::mat4 projection_{1.0f};
    glm::vec2 whiteUV_{0.0f};
    CharacterUV characterUV_[3];
    std::vector<Vertex> vertices_;
};

} // namespace warva
