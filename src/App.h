#pragma once

#include "Input.h"
#include "Renderer.h"
#include "World.h"

struct GLFWwindow;

namespace warva {

class App {
public:
    App() = default;
    ~App();

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    int run();

private:
    void shutdown();
    void updateTitle(double fps);

    GLFWwindow* window_ = nullptr;
    Input input_;
    World world_;
    Renderer renderer_;
};

} // namespace warva
