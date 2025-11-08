#pragma once

#include <glm/fwd.hpp>

#include "sokka/graphics/Camera.hpp"
#include "sokka/graphics/Shader.hpp"

namespace Sokka {

class Renderer {
public:
    static void init();
    static void close();

    static void startScene(const Camera& camera);
    static void endScene();

    static void drawLine(const glm::vec3& a, glm::vec3& b, const glm::vec4& color);
    static void drawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

private:
    static Shared<Shader> viewShader_;
    static Shared<Shader> spriteShader_;
    static unsigned int vao_, vbo_;
};

}  // namespace Sokka
