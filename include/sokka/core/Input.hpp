#pragma once

#include <SDL_events.h>
#include <SDL_mouse.h>
#include <SDL_scancode.h>

#include "sokka/graphics/Vector2.hpp"

namespace Sokka {

class Input {
public:
    static void processEvent(const SDL_Event* event);
    static void update();

    static bool isKeyPressed(SDL_Scancode key);
    static bool isMouseButtonPressed(Uint8 button = SDL_BUTTON_LEFT);

    static Vector2 getMousePosition();
    static Vector2 getMouseDelta();

    static float getMouseX();
    static float getMouseY();
    static float getMouseWheelY();

private:
    static inline Vector2 mousePosition_{0.0f, 0.0f};
    static inline Vector2 lastMousePosition_{0.0f, 0.0f};
    static inline Vector2 mouseDelta_{0.0f, 0.0f};

    static inline int mouseWheelY_ = 0;
};
}  // namespace Sokka
