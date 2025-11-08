#include "sokka/ui/Canvas.hpp"

#include <GL/glew.h>
#include <imgui.h>

#include "sokka/core/Base.hpp"
#include "sokka/core/Logger.hpp"
#include "sokka/graphics/Camera.hpp"
#include "sokka/graphics/Vector2.hpp"

namespace Sokka {

Canvas::Canvas() : UIPanel("Scene"), camera_() {
    if (!init()) {
        SOKKA_ERROR("Failed to initialise Canvas");
    }
    SOKKA_SUCCESS("Initialised Canvas");
}

bool Canvas::init() {
    // You might initialize your framebuffer here if you render to texture
    camera_.setViewport(1280, 720);
    shader_ = Shader::create("Canvas", VERTEX_SHADER, FRAGMENT_SHADER);

    // --- Create framebuffer ---
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

    // color texture
    glGenTextures(1, &colorTexture_);
    glBindTexture(GL_TEXTURE_2D, colorTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture_, 0);

    // depth-stencil buffer
    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        SOKKA_ERROR("Framebuffer incomplete!");
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void Canvas::update(float deltaTime) {
    if (isHovered_) {
        camera_.update(deltaTime);
    }
}

void Canvas::render() {
    // bind shader and draw scene

    // 1. Bind FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glViewport(0, 0, 1280, 720);

    // 2. Clear and draw your scene normally
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_->bind();
    shader_->setMat4("u_ViewProjection", camera_.getViewProjection());

    // render scene objects
    static unsigned int vao = 0, vbo = 0;
    if (vao == 0) {
        float vertices[] = {-100.0f, -100.0f, 100.0f, -100.0f, 0.0f, 100.0f};
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    }

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    shader_->unbind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Draw ImGui window
    ImGui::Begin("Scene");
    isHovered_ = ImGui::IsWindowHovered();
    ImVec2 size = ImGui::GetContentRegionAvail();
    // SOKKA_INFO("Size: ", size.x, ", ", size.y);

    ImGui::Image((ImTextureID)(uintptr_t)colorTexture_, size, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}

Unique<Canvas> Canvas::create() {
    // create Canvas
    return makeUnique<Canvas>();
}

}  // namespace Sokka
