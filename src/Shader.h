#pragma once

#include <glad/glad.h>

#include <string>

namespace warva {

class Shader {
public:
    Shader() = default;
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    bool build(const char* vertexSource, const char* fragmentSource, std::string& error);
    void destroy();

    void use() const;
    GLuint id() const { return program_; }
    GLint uniformLocation(const char* name) const;

private:
    GLuint program_ = 0;
};

} // namespace warva
