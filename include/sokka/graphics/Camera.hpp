#pragma once

#include "sokka/graphics/Vector2.hpp"
namespace Sokka {

class Camera {
public:
    Camera() = default;
    ~Camera() = default;

    void update();

    void setVeiwport(int width, int height) {
        viewport_.width = width;
        viewport_.height = height;
    }

    float getDistance() const { return zoom_; }
    void setDistance(float distance) { zoom_ = distance; }

    Vector2 getPosition() const { return position_; }
    void setPosition(const Vector2& position) { position_ = position; }

    void move(const Vector2& delta) { position_ += delta; }
    void zoom(float factor) { zoom_ *= factor; }

    Vector2 worldToScreen(const Vector2& world) const;
    Vector2 screenToWorld(const Vector2& screen) const;

private:
    static constexpr unsigned int DEFAULT_VIEWPORT_WIDTH = 1280;
    static constexpr unsigned int DEFAULT_VIEWPORT_HEIGHT = 720;

    struct Viewport {
        unsigned int width = DEFAULT_VIEWPORT_WIDTH;
        unsigned int height = DEFAULT_VIEWPORT_HEIGHT;
    };

    float zoom_ = 10.0f;
    Vector2 position_ = {0, 0};
    Viewport viewport_;
};
}  // namespace Sokka
