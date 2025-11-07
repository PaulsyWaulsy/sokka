#include "sokka/graphics/Camera.hpp"

namespace Sokka {

Vector2 Camera::worldToScreen(const Vector2& world) const {
    return {(world.x - position_.x) * zoom_, (world.y - position_.y) * zoom_};
}

Vector2 Camera::screenToWorld(const Vector2& screen) const {
    return {screen.x / zoom_ + position_.x, screen.y / zoom_ + position_.y};
}
}  // namespace Sokka
