#pragma once

#include <glad/glad.h>

#include <string>

namespace warva {

class Texture {
public:
    Texture() = default;
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    bool createRGBA(int width, int height, const unsigned char* pixels, std::string& error);
    bool loadFromFile(const std::string& path, std::string& error);
    void destroy();

    void bind(GLenum unit = GL_TEXTURE0) const;
    GLuint id() const { return texture_; }
    int width() const { return width_; }
    int height() const { return height_; }

private:
    GLuint texture_ = 0;
    int width_ = 0;
    int height_ = 0;
};

} // namespace warva
