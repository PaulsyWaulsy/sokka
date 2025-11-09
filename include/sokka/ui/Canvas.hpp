#pragma once

#include <GL/glew.h>
#include <SDL_events.h>

#include "imgui.h"
#include "sokka/core/Base.hpp"
#include "sokka/editor/RoomTool.hpp"
#include "sokka/graphics/Camera.hpp"
#include "sokka/graphics/FrameBuffer.hpp"
#include "sokka/graphics/Shader.hpp"
#include "sokka/graphics/Vector2.hpp"
#include "sokka/ui/UIPanel.hpp"

namespace Sokka {

class Canvas : public UIPanel {
public:
    Canvas();

    bool init() override;
    void render() override;

    // TODO: make an event callback system
    void processEvent(const SDL_Event* event);
    void update(float deltaTime);

    static Unique<Canvas> create();

private:
    bool isMouseOver() const;
    void resizeBuffer(int width, int height);
    void resizeFrameBuffer(int width, int height);
    bool initFrameBuffer();

    void renderGUI();
    void renderStart();
    void renderEnd();

    bool initTools();

private:
    // for hover logic
    bool isHovered_ = false;
    ImVec2 size_ = {DEFAULT_VIEWPORT_WIDTH, DEFAULT_VIEWPORT_HEIGHT};
    Vector2 mouseWorld_ = {0, 0};

    Shared<FrameBuffer> frameBuffer_;

    Viewport viewport_;
    Camera camera_;
    Shared<Shader> shader_;

    // TEST:
    Unique<RoomTool> roomTool_;

    static constexpr const char* VERTEX_SHADER = "assets/shaders/basic.vert";
    static constexpr const char* FRAGMENT_SHADER = "assets/shaders/basic.frag";
};
}  // namespace Sokka
//
