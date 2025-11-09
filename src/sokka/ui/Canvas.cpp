#include "sokka/ui/Canvas.hpp"

#include <GL/glew.h>
#include <SDL_events.h>
#include <SDL_mouse.h>
#include <imgui.h>

#include "sokka/core/Base.hpp"
#include "sokka/core/Input.hpp"
#include "sokka/core/Logger.hpp"
#include "sokka/editor/RoomTool.hpp"
#include "sokka/graphics/Camera.hpp"
#include "sokka/graphics/FrameBuffer.hpp"
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
    frameBuffer_ = FrameBuffer::create(DEFAULT_VIEWPORT_WIDTH, DEFAULT_VIEWPORT_HEIGHT);

    if (!initTools()) {
        SOKKA_ERROR("Failed to setup canvas tools");
        return false;
    }

    return true;
}

void Canvas::update(float deltaTime) {
    if (isHovered_) {
        if (Input::isKeyPressed(SDL_SCANCODE_LCTRL)) {
            camera_.update(deltaTime);
        } else {
            roomTool_->onMouseMove(mouseWorld_);
        }
    }
}

// FIX: remove this mess
void Canvas::processEvent(const SDL_Event* event) {
    if (isHovered_) {
        switch (event->type) {
            case SDL_MOUSEBUTTONDOWN:
                if (event->button.button == SDL_BUTTON_LEFT) {
                    roomTool_->onMouseDown(mouseWorld_);
                    SOKKA_INFO("Mouse Button Down");
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event->button.button == SDL_BUTTON_LEFT) {
                    roomTool_->onMouseUp(mouseWorld_);
                    SOKKA_INFO("Mouse Button up");
                }
                break;
            default:
                break;
        }
    }
}

void Canvas::renderGUI() {
    if (isHovered_) {
        // Get ImGui info
        // ImVec2 windowPos = ImGui::GetWindowPos();
        // ImVec2 contentRegion = ImGui::GetWindowContentRegionMin();
        ImVec2 panelPosition = ImGui::GetCursorScreenPos();  // top-left of image in screen space
        ImVec2 panelSize = ImGui::GetContentRegionAvail();   // size of the panel region

        // Mouse in screen coords
        float mouseX = Input::getMouseX();
        float mouseY = Input::getMouseY() - 70;  // FIX: this is the width from title bar

        // Convert to position relative to image
        float localX = mouseX - panelPosition.x;
        float localY = mouseY - panelPosition.y;

        // Flip Y (ImGui top-left → OpenGL bottom-left)
        localY = panelSize.y - localY;

        // Convert to NDC
        float ndcX = (localX / panelSize.x) * 2.0f - 1.0f;
        float ndcY = (localY / panelSize.y) * 2.0f - 1.0f;

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
    renderStart();  // Start Scene

    Renderer::drawRect({0.0f, 0.0f, 0.0f}, {8.0f, 8.0f}, {0.2f, 0.8f, 0.2f, 1.0f});
    Renderer::drawRect({mouseWorld_.x, mouseWorld_.y, 0.0f}, {8.0f, 8.0f},
                       {1.0f, 0.0f, 0.0f, 0.5f});

    roomTool_->render();

    renderGUI();

    renderEnd();  // End Scene
}

void Canvas::renderStart() {
    // begin ImGui window
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    ImGui::Begin("Scene", nullptr, flags);
    isHovered_ = ImGui::IsWindowHovered();
    size_ = ImGui::GetContentRegionAvail();

    if ((int)size_.x != viewport_.width || (int)size_.y != viewport_.height) {
        frameBuffer_->resize((int)size_.x, (int)size_.y);
        camera_.setViewport((int)size_.x, (int)size_.y);
    }

    // 1. Bind FBO
    frameBuffer_->bind();
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Renderer::startScene(camera_);
}

void Canvas::renderEnd() {
    Renderer::endScene();
    frameBuffer_->unbind();
    GLuint textureId = frameBuffer_->getColorAttachment();
    ImGui::Image((ImTextureID)(uintptr_t)textureId, size_, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

bool Canvas::initTools() {
    // create tools
    roomTool_ = RoomTool::create(*this);
    return true;
}

Unique<Canvas> Canvas::create() {
    // create Canvas
    return makeUnique<Canvas>();
}

}  // namespace Sokka
