#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "sokka/core/Logger.hpp"
#include "sokka/graphics/Vector2.hpp"
namespace Sokka {

class Camera {
public:
    Camera(int width = DEFAULT_VIEWPORT_WIDTH, int height = DEFAULT_VIEWPORT_HEIGHT);
    ~Camera() = default;

    void update(float deltaTime);

    void setViewport(int width, int height) {
        viewport_.width = width;
        viewport_.height = height;
    }

    float getDistance() const { return zoom_; }
    void setDistance(float distance) { zoom_ = distance; }

    Vector2 getPosition() const { return position_; }
    void setPosition(const Vector2& position) { position_ = position; }

    const glm::mat4& getView() const { return viewMatrix_; }
    const glm::mat4& getProjection() const { return projectionMatrix_; }
    const glm::mat4& getViewProjection() const { return viewProjectionMatrix_; }

private:
    void updateView();
    void move(const Vector2& delta) {
        SOKKA_INFO("Camera Move!");
        position_ += delta;
    }
    void zoom(float factor) {
        SOKKA_INFO("Camera zoom!");
        zoom_ *= factor;
    }

    static constexpr unsigned int DEFAULT_VIEWPORT_WIDTH = 1280;
    static constexpr unsigned int DEFAULT_VIEWPORT_HEIGHT = 720;

    struct Viewport {
        int width = DEFAULT_VIEWPORT_WIDTH;
        int height = DEFAULT_VIEWPORT_HEIGHT;
    };

    float zoom_ = 10.0f;
    Vector2 position_ = {0, 0};
    Viewport viewport_;

    glm::mat4 viewMatrix_{1.0f};
    glm::mat4 projectionMatrix_{1.0f};
    glm::mat4 viewProjectionMatrix_{1.0f};
};
}  // namespace Sokka
