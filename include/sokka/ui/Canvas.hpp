#pragma once

#include <GL/glew.h>

#include "imgui.h"
#include "sokka/core/Base.hpp"
#include "sokka/graphics/Camera.hpp"
#include "sokka/graphics/Shader.hpp"
#include "sokka/graphics/Vector2.hpp"
#include "sokka/ui/UIPanel.hpp"

namespace Sokka {

class Canvas : public UIPanel {
public:
    Canvas();

    bool init() override;
    void render() override;
    void update(float deltaTime);

    static Unique<Canvas> create();

private:
    void renderGUI();
    bool isMouseOver() const;
    void resizeBuffer(int width, int height);
    ImVec2 getPanelSize();
    void resizeFrameBuffer(int width, int height);

    // for hover logic
    bool isHovered_ = false;
    Vector2 size_ = {DEFAULT_VIEWPORT_WIDTH, DEFAULT_VIEWPORT_HEIGHT};
    Vector2 mouseWorld_ = {0, 0};

    // TODO: refactor into frameBuffer class
    GLuint fbo_ = 0;
    GLuint colorTexture_ = 0;
    GLuint rbo_ = 0;  // depth-stencil

    Viewport viewport_;
    Camera camera_;
    Shared<Shader> shader_;

    static constexpr const char* VERTEX_SHADER = "assets/shaders/basic.vert";
    static constexpr const char* FRAGMENT_SHADER = "assets/shaders/basic.frag";
};
}  // namespace Sokka
//
