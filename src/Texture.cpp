#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Texture.h"

namespace warva {

Texture::~Texture() {
    destroy();
}

bool Texture::createRGBA(int width, int height, const unsigned char* pixels,
                         std::string& error) {
    if (width <= 0 || height <= 0 || pixels == nullptr) {
        error = "invalid texture data";
        return false;
    }

    destroy();
    glGenTextures(1, &texture_);
    if (texture_ == 0) {
        error = "glGenTextures failed";
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, texture_);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    width_ = width;
    height_ = height;
    return true;
}

bool Texture::loadFromFile(const std::string& path, std::string& error) {
    int width = 0;
    int height = 0;
    int channels = 0;

    stbi_set_flip_vertically_on_load(0);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
    if (data == nullptr) {
        const char* reason = stbi_failure_reason();
        error = "failed to load '" + path + "': " +
                (reason != nullptr ? reason : "unknown");
        return false;
    }

    const bool ok = createRGBA(width, height, data, error);
    stbi_image_free(data);
    return ok;
}

void Texture::destroy() {
    if (texture_ != 0) {
        glDeleteTextures(1, &texture_);
        texture_ = 0;
    }
    width_ = 0;
    height_ = 0;
}

void Texture::bind(GLenum unit) const {
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, texture_);
}

} // namespace warva
