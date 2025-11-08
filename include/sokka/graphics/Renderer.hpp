#pragma once

#include "GL/glew.h"
#include "sokka/graphics/Camera.hpp"
#include "sokka/graphics/Shader.hpp"

namespace Sokka {

class Renderer {
public:
    static void init();
    static void close();

    static void startScene(const Camera& camera);
    static void endScene();

    static void drawLine(const glm::vec3& a, const glm::vec3& b, const glm::vec4& color);
    static void drawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

private:
    static constexpr const char* VIEW_VERTEX_SHADER = "assets/shaders/basic.vert";
    static constexpr const char* VIEW_FRAGMENT_SHADER = "assets/shaders/basic.frag";
    static constexpr const char* SPRITE_VERTEX_SHADER = "assets/shaders/sprite.frag";
    static constexpr const char* SPRITE_FRAGMENT_SHADER = "assets/shaders/sprite.frag";

    static Shared<Shader> viewShader_;
    static Shared<Shader> spriteShader_;
    static glm::mat4 viewProjection_;
    static unsigned int quadVAO_, quadVBO_;
    static unsigned int lineVAO_, lineVBO_;

    static GLuint fbo_, colorTexture_, rbo_;
};

}  // namespace Sokka
