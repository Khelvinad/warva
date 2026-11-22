#include "Input.h"

namespace warva {

void Input::attach(GLFWwindow* window) {
    window_ = window;
}

void Input::beginFrame() {
    if (window_ == nullptr) return;

    glfwGetCursorPos(window_, &mouseX_, &mouseY_);
    glfwGetWindowSize(window_, &windowWidth_, &windowHeight_);

    const bool downNow =
        glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    if (downNow && !down_) {
        clicked_ = true;
        pressTime_ = glfwGetTime();
    }
    down_ = downNow;
}

void Input::endFrame() {
    // A click that was not consumed this frame is discarded so it cannot fire
    // later on an unrelated tile.
    clicked_ = false;
}

bool Input::consumeClick() {
    if (!clicked_) return false;
    clicked_ = false;
    return true;
}

bool Input::mouseDown() const {
    return down_;
}

double Input::holdDuration() const {
    if (!down_ || window_ == nullptr) return 0.0;
    return glfwGetTime() - pressTime_;
}

ScreenPoint Input::mouseVirtual() const {
    if (window_ == nullptr || windowWidth_ <= 0 || windowHeight_ <= 0) {
        return {};
    }

    const float width = static_cast<float>(windowWidth_);
    const float height = static_cast<float>(windowHeight_);
    const float targetAspect = kVirtualWidth / kVirtualHeight;

    float viewWidth = 0.0f;
    float viewHeight = 0.0f;
    float offsetX = 0.0f;
    float offsetY = 0.0f;

    if (width / height > targetAspect) {
        viewHeight = height;
        viewWidth = viewHeight * targetAspect;
        offsetX = (width - viewWidth) * 0.5f;
    } else {
        viewWidth = width;
        viewHeight = viewWidth / targetAspect;
        offsetY = (height - viewHeight) * 0.5f;
    }

    return {
        (static_cast<float>(mouseX_) - offsetX) / viewWidth * kVirtualWidth,
        (static_cast<float>(mouseY_) - offsetY) / viewHeight * kVirtualHeight,
    };
}

} // namespace warva
