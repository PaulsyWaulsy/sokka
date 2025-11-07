#include "sokka/ui/Canvas.hpp"

#include <GL/glew.h>
#include <imgui.h>

#include "sokka/core/Base.hpp"
#include "sokka/core/Input.hpp"
#include "sokka/core/Logger.hpp"
#include "sokka/graphics/Camera.hpp"
#include "sokka/graphics/Vector2.hpp"

namespace Sokka {

Canvas::Canvas() : UIPanel("Scene"), camera_() {
    if (!init()) {
        SOKKA_ERROR("Failed to initialise Canvas");
    }
}

bool Canvas::init() {
    // You might initialize your framebuffer here if you render to texture
    camera_.setViewport(1280, 720);  // default, will be overridden by ImGui panel size
    return true;
}

void Canvas::update(float deltaTime) {
    if (isHovered_) {
        camera_.update(deltaTime);
    }
}

void Canvas::render() {
    ImGui::Begin("Scene");

    isHovered_ = ImGui::IsWindowHovered();

    ImGui::End();
}

Unique<Canvas> Canvas::create() {
    // create Canvas
    return makeUnique<Canvas>();
}

}  // namespace Sokka
