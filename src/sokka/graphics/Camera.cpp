#include "sokka/graphics/Camera.hpp"

#include <SDL_mouse.h>
#include <SDL_scancode.h>

#include <algorithm>

#include "sokka/core/Input.hpp"
#include "sokka/core/Logger.hpp"
#include "sokka/graphics/Vector2.hpp"

namespace Sokka {
Camera::Camera(int width, int height) : zoom_(1.0f), position_({0, 0}), viewport_({width, height}) {
    if (width < 0) SOKKA_WARN("Camera width should be positive: ", width);
    if (height < 0) SOKKA_WARN("Camera height should be positive: ", height);

    SOKKA_SUCCESS("Created editor camera");
    updateView();
}

void Camera::update(float deltaTime) {
    (void)deltaTime;
    // get mouse movement delta

    float scroll = Input::getMouseWheelY();
    // SOKKA_INFO("Scrolled: ", scroll);

    Vector2 delta = Input::getMouseDelta() * deltaTime;
    if (Input::isMouseButtonPressed(SDL_BUTTON_LEFT)) {
        move(delta);
    } else if (scroll != 0) {
        zoom(scroll);
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

void Camera::move(const Vector2& delta) {
    float speed = CAMERA_MOVE_SPEED / zoom_;

    position_.x -= delta.x * speed;
    position_.y += delta.y * speed;
}

void Camera::zoom(float scroll) {
    if (scroll > 0) {
        zoom_ /= CAMERA_ZOOM_FACTOR;
    } else if (scroll < 0) {
        zoom_ *= CAMERA_ZOOM_FACTOR;
    }
    zoom_ = std::clamp(zoom_, MIN_ZOOM, MAX_ZOOM);
}

}  // namespace Sokka
//
