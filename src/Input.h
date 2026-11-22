#pragma once

#include "Iso.h"

#include <GLFW/glfw3.h>

namespace warva {

// Polls GLFW input once per rendered frame. Clicks are edge-triggered and are
// consumed by gameplay; hold duration is exposed for future hold-to-clean
// interactions (WV-020).
class Input {
public:
    void attach(GLFWwindow* window);

    void beginFrame();
    void endFrame();

    bool consumeClick();
    bool mouseDown() const;
    double holdDuration() const;

    // Mouse position mapped from window coordinates into the 1280x720 virtual
    // space, including letterbox offsets.
    ScreenPoint mouseVirtual() const;

private:
    GLFWwindow* window_ = nullptr;
    double mouseX_ = 0.0;
    double mouseY_ = 0.0;
    int windowWidth_ = 1280;
    int windowHeight_ = 720;
    bool down_ = false;
    bool clicked_ = false;
    double pressTime_ = 0.0;
};

} // namespace warva
