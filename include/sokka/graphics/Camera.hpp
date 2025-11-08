#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>

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
        updateView();
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
    std::pair<float, float> calculateMoveSpeed() const;
    float calculateZoomSpeed() const;
    void move(const Vector2& delta);
    void zoom(float scroll);

    static constexpr unsigned int DEFAULT_VIEWPORT_WIDTH = 1280;
    static constexpr unsigned int DEFAULT_VIEWPORT_HEIGHT = 720;
    static constexpr float CAMERA_MOVE_SPEED = 1000.0f;
    static constexpr float CAMERA_ZOOM_FACTOR = 1.1f;
    static constexpr float MIN_ZOOM = 0.1f;
    static constexpr float MAX_ZOOM = 50.0f;

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
