#include "sokka/graphics/Renderer.hpp"

#include "GL/glew.h"

namespace Sokka {

Shared<Shader> Renderer::viewShader_ = nullptr;
glm::mat4 Renderer::viewProjection_ = glm::mat4(1.0f);

unsigned int Renderer::lineVAO_ = 0;
unsigned int Renderer::lineVBO_ = 0;

unsigned int Renderer::quadVAO_ = 0;
unsigned int Renderer::quadVBO_ = 0;

void Renderer::init() {
    viewShader_ = Shader::create(VIEW_VERTEX_SHADER, VIEW_FRAGMENT_SHADER);

    // --- line buffers ---
    glGenVertexArrays(1, &lineVAO_);
    glGenBuffers(1, &lineVBO_);
    glBindVertexArray(lineVAO_);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // --- Quad (for rects + sprites) ---
    float vertices[] = {
        // pos(x,y,z)   // uv
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    glGenVertexArrays(1, &quadVAO_);
    glGenBuffers(1, &quadVBO_);
    glBindVertexArray(quadVAO_);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::close() {
    glDeleteVertexArrays(1, &lineVAO_);
    glDeleteBuffers(1, &lineVBO_);
    glDeleteVertexArrays(1, &quadVAO_);
    glDeleteBuffers(1, &quadVBO_);
}

void Renderer::startScene(const Camera& camera) {
    viewProjection_ = camera.getViewProjection();

    viewShader_->bind();
    viewShader_->setMat4("u_ViewProjection", viewProjection_);
}

void Renderer::endScene() { viewShader_->unbind(); }

void Renderer::drawLine(const glm::vec3& a, const glm::vec3& b, const glm::vec4& color) {
    viewShader_->setFloat4("u_Color", color);

    float vertices[6] = {a.x, a.y, a.z, b.x, b.y, b.z};

    glBindBuffer(GL_ARRAY_BUFFER, lineVBO_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindVertexArray(lineVAO_);

    glDrawArrays(GL_LINES, 0, 2);
}

void Renderer::drawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
    viewShader_->setFloat4("u_Color", color);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::scale(model, glm::vec3(size, 1.0f));
    viewShader_->setMat4("u_Model", model);

    glBindVertexArray(quadVAO_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

}  // namespace Sokka
