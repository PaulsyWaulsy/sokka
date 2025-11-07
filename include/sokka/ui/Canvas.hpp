#pragma once

#include "sokka/core/Base.hpp"
#include "sokka/graphics/Camera.hpp"
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

    Camera camera_;
};
}  // namespace Sokka
//
