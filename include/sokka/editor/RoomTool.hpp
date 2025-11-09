#pragma once

#include "sokka/core/Base.hpp"
#include "sokka/editor/Tool.hpp"
#include "sokka/graphics/Renderer.hpp"
#include "sokka/graphics/Vector2.hpp"

namespace Sokka {

// TODO: refactor into cpp
class RoomTool : public Tool {
public:
    RoomTool(Canvas& canvas) : Tool(canvas, "Room") {}

    void onMouseDown(const Vector2& worldPosition) override {
        start_ = worldPosition;
        dragging_ = true;
    }

    void onMouseMove(const Vector2& worldPosition) override {
        if (dragging_) end_ = worldPosition;
    }

    void onMouseUp(const Vector2& worldPosition) override {
        if (dragging_) {
            end_ = worldPosition;
            dragging_ = false;
            // Add room data to level manager
        }
    }

    void render() override {
        if (dragging_) {
            glm::vec3 position = {start_.x, start_.y, 0.0f};
            glm::vec2 size = {end_.x - start_.x, end_.y - start_.y};
            Renderer::drawRect(position, size, {0.3f, 0.6f, 1.0f, 0.3f});
        }
    }

    void setDraging(bool enable) { dragging_ = enable; }

    static Unique<RoomTool> create(Canvas& canvas) { return makeUnique<RoomTool>(canvas); }

private:
    bool dragging_ = false;
    Vector2 start_;
    Vector2 end_;
};
}  // namespace Sokka
