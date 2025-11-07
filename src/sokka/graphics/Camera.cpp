#include "sokka/graphics/Camera.hpp"

#include <SDL_mouse.h>
#include <SDL_scancode.h>

#include "sokka/core/Input.hpp"
#include "sokka/core/Logger.hpp"
#include "sokka/graphics/Vector2.hpp"

namespace Sokka {
Camera::Camera(int width, int height) : zoom_(1.0f), position_({0, 0}), viewport_({width, height}) {
    if (width < 0) SOKKA_WARN("Camera width should be positive: ", width);
    if (height < 0) SOKKA_WARN("Camera height should be positive: ", height);

    updateView();
}

void Camera::update(float deltaTime) {
    (void)deltaTime;
    if (Input::isKeyPressed(SDL_SCANCODE_LCTRL)) {
        // get mouse movement delta

        float scroll = Input::getMouseWheelY();
        Vector2 delta = Input::getMouseDelta() * deltaTime;
        if (Input::isMouseButtonPressed(SDL_BUTTON_LEFT)) {
            move(delta);
        } else if (scroll != 0) {
            zoom(scroll);
        }
    }

    updateView();
}

void Camera::updateView() {
    // 1. Create orthographic projection centered on (0,0)
    float halfW = (viewport_.width * 0.5f) / zoom_;
    float halfH = (viewport_.height * 0.5f) / zoom_;

    projectionMatrix_ = glm::ortho(-halfW, halfW, -halfH, halfH, -1.0f, 1.0f);
    viewMatrix_ = glm::translate(glm::mat4(1.0f), glm::vec3(-position_.x, -position_.y, 0.0f));
    viewProjectionMatrix_ = projectionMatrix_ * viewMatrix_;
}

}  // namespace Sokka
