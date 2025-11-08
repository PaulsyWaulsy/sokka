#include "sokka/ui/Canvas.hpp"

#include <GL/glew.h>
#include <imgui.h>

#include "sokka/core/Base.hpp"
#include "sokka/core/Input.hpp"
#include "sokka/core/Logger.hpp"
#include "sokka/graphics/Camera.hpp"
#include "sokka/graphics/Renderer.hpp"

namespace Sokka {

Canvas::Canvas() : UIPanel("Scene"), camera_() {
    if (!init()) {
        SOKKA_ERROR("Failed to initialise Canvas");
    }
    SOKKA_SUCCESS("Initialised Canvas");
}

bool Canvas::init() {
    // You might initialize your framebuffer here if you render to texture
    camera_.setViewport(DEFAULT_VIEWPORT_WIDTH, DEFAULT_VIEWPORT_HEIGHT);
    shader_ = Shader::create(VERTEX_SHADER, FRAGMENT_SHADER);

    // --- Create framebuffer ---
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

    // color texture
    glGenTextures(1, &colorTexture_);
    glBindTexture(GL_TEXTURE_2D, colorTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewport_.width, viewport_.height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture_, 0);

    // depth-stencil buffer
    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewport_.width, viewport_.height);
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

void Canvas::renderGUI() {
    if (isHovered_) {
        // Get ImGui info
        // ImVec2 windowPos = ImGui::GetWindowPos();
        // ImVec2 contentRegion = ImGui::GetWindowContentRegionMin();
        ImVec2 imagePosition = ImGui::GetCursorScreenPos();  // top-left of image in screen space
        ImVec2 imageSize = ImGui::GetContentRegionAvail();   // size of the panel region

        // Mouse in screen coords
        float mouseX = Input::getMouseX();
        float mouseY = Input::getMouseY() - 70;  // FIX: this is the width from title bar

        // Convert to position relative to image
        float localX = mouseX - imagePosition.x;
        float localY = mouseY - imagePosition.y;

        // Flip Y (ImGui top-left → OpenGL bottom-left)
        localY = imageSize.y - localY;

        // Convert to NDC
        float ndcX = (localX / imageSize.x) * 2.0f - 1.0f;
        float ndcY = (localY / imageSize.y) * 2.0f - 1.0f;

        // Convert to world
        glm::mat4 invViewProj = glm::inverse(camera_.getViewProjection());
        glm::vec4 clipPos = glm::vec4(ndcX, ndcY, 0.0f, 1.0f);
        glm::vec4 worldPos = invViewProj * clipPos;
        worldPos /= worldPos.w;

        mouseWorld_ = {worldPos.x, worldPos.y};
        mouseWorld_.x = std::floor(worldPos.x / TILE_SIZE) * TILE_SIZE;
        mouseWorld_.y = std::floor(worldPos.y / TILE_SIZE) * TILE_SIZE;

        ImGui::Separator();
        ImGui::Text("Mouse screen: [%.1f, %.1f]", mouseX, mouseY);
        ImGui::Text("Mouse (panel): [%.1f, %.1f]", localX, localY);
        ImGui::Text("Mouse (world): [%.2f, %.2f]", worldPos.x, worldPos.y);
    }
}

void Canvas::render() {
    // begin ImGui window
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    ImGui::Begin("Scene", nullptr, flags);
    isHovered_ = ImGui::IsWindowHovered();
    ImVec2 size = ImGui::GetContentRegionAvail();

    if ((int)size.x != viewport_.width || (int)size.y != viewport_.height) {
        resizeFrameBuffer((int)size.x, (int)size.y);
        camera_.setViewport((int)size.x, (int)size.y);
    }

    // 1. Bind FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glViewport(0, 0, viewport_.width, viewport_.height);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Renderer::startScene(camera_);

    Renderer::drawRect({0.0f, 0.0f, 0.0f}, {8.0f, 8.0f}, {0.2f, 0.8f, 0.2f, 1.0f});

    Renderer::drawRect({mouseWorld_.x, mouseWorld_.y, 0.0f}, {8.0f, 8.0f},
                       {1.0f, 0.0f, 0.0f, 0.5f});

    Renderer::endScene();

    // renderGUI
    renderGUI();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ImGui::Image((ImTextureID)(uintptr_t)colorTexture_, size, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

void Canvas::resizeFrameBuffer(int width, int height) {
    viewport_.width = width;
    viewport_.height = height;

    glBindTexture(GL_TEXTURE_2D, colorTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

Unique<Canvas> Canvas::create() {
    // create Canvas
    return makeUnique<Canvas>();
}

}  // namespace Sokka
