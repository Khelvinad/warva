#include "Shader.h"

#include <algorithm>
#include <cstddef>
#include <vector>

namespace {

GLuint compileStage(GLenum type, const char* source, std::string& error) {
    const GLuint shader = glCreateShader(type);
    if (shader == 0) {
        error = "glCreateShader failed";
        return 0;
    }

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(static_cast<std::size_t>(std::max(length, 1)));
        glGetShaderInfoLog(shader, length, nullptr, log.data());
        error = log.data();
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

} // namespace

namespace warva {

Shader::~Shader() {
    destroy();
}

bool Shader::build(const char* vertexSource, const char* fragmentSource,
                   std::string& error) {
    destroy();

    const GLuint vertex = compileStage(GL_VERTEX_SHADER, vertexSource, error);
    if (vertex == 0) return false;

    const GLuint fragment = compileStage(GL_FRAGMENT_SHADER, fragmentSource, error);
    if (fragment == 0) {
        glDeleteShader(vertex);
        return false;
    }

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        GLint length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(static_cast<std::size_t>(std::max(length, 1)));
        glGetProgramInfoLog(program, length, nullptr, log.data());
        error = log.data();
        glDeleteProgram(program);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return false;
    }

    glDetachShader(program, vertex);
    glDetachShader(program, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    program_ = program;
    return true;
}

void Shader::destroy() {
    if (program_ != 0) {
        glDeleteProgram(program_);
        program_ = 0;
    }
}

void Shader::use() const {
    glUseProgram(program_);
}

GLint Shader::uniformLocation(const char* name) const {
    return glGetUniformLocation(program_, name);
}

} // namespace warva
