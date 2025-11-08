#pragma once

#include <GL/glew.h>

#include "sokka/core/Base.hpp"
#include "sokka/graphics/Camera.hpp"
#include "sokka/graphics/Shader.hpp"
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
    bool isMouseOver() const;

    // for hover logic
    bool isHovered_ = false;

    GLuint fbo_ = 0;
    GLuint colorTexture_ = 0;
    GLuint rbo_ = 0;  // depth-stencil

    Camera camera_;
    Shared<Shader> shader_;

    static constexpr const char* VERTEX_SHADER = "assets/shaders/basic.vert";
    static constexpr const char* FRAGMENT_SHADER = "assets/shaders/basic.frag";
};
}  // namespace Sokka
//
