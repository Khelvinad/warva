#include "App.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <string>

namespace {

constexpr double kFixedStep = 1.0 / 60.0;
constexpr double kMaxFrameTime = 0.25;

void glfwErrorCallback(int code, const char* description) {
    std::fprintf(stderr, "GLFW error %d: %s\n", code, description);
}

} // namespace

namespace warva {

App::~App() {
    shutdown();
}

int App::run() {
    glfwSetErrorCallback(glfwErrorCallback);
    if (glfwInit() != GLFW_TRUE) {
        std::fprintf(stderr, "Failed to initialize GLFW\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    window_ = glfwCreateWindow(1280, 720, "WARVA", nullptr, nullptr);
    if (window_ == nullptr) {
        std::fprintf(stderr, "Failed to create the GLFW window\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
        std::fprintf(stderr, "Failed to load OpenGL functions with GLAD\n");
        shutdown();
        return 1;
    }

    std::printf("OpenGL %s | %s\n",
                reinterpret_cast<const char*>(glGetString(GL_VERSION)),
                reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

    input_.attach(window_);

    std::string error;
    if (!renderer_.init(error)) {
        std::fprintf(stderr, "Renderer initialization failed: %s\n", error.c_str());
        shutdown();
        return 1;
    }

    double lastTime = glfwGetTime();
    double accumulator = 0.0;
    double titleTimer = 0.0;
    int frames = 0;

    while (glfwWindowShouldClose(window_) == 0) {
        glfwPollEvents();
        input_.beginFrame();

        const bool focused = glfwGetWindowAttrib(window_, GLFW_FOCUSED) == GLFW_TRUE;
        const double now = glfwGetTime();
        double frameTime = now - lastTime;
        lastTime = now;
        if (frameTime > kMaxFrameTime) frameTime = kMaxFrameTime;

        if (focused) {
            world_.handleInput(input_);
            accumulator += frameTime;
            while (accumulator >= kFixedStep) {
                world_.update(static_cast<float>(kFixedStep));
                accumulator -= kFixedStep;
            }
        } else {
            // Pause the simulation when the window loses focus.
            accumulator = 0.0;
        }
        input_.endFrame();

        int framebufferWidth = 0;
        int framebufferHeight = 0;
        glfwGetFramebufferSize(window_, &framebufferWidth, &framebufferHeight);
        if (framebufferWidth > 0 && framebufferHeight > 0) {
            renderer_.render(world_, framebufferWidth, framebufferHeight);
        }
        glfwSwapBuffers(window_);

        ++frames;
        titleTimer += frameTime;
        if (titleTimer >= 0.5) {
            updateTitle(static_cast<double>(frames) / titleTimer);
            frames = 0;
            titleTimer = 0.0;
        }
    }

    shutdown();
    return 0;
}

void App::shutdown() {
    if (window_ == nullptr) return;
    renderer_.shutdown();
    glfwDestroyWindow(window_);
    window_ = nullptr;
    glfwTerminate();
}

void App::updateTitle(double fps) {
    char title[128];
    std::snprintf(title, sizeof(title), "WARVA | %.0f FPS | klik tile untuk bergerak", fps);
    glfwSetWindowTitle(window_, title);
}

} // namespace warva
