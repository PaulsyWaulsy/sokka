#include "sokka/core/Input.hpp"

#include <SDL2/SDL.h>

namespace Sokka {

void Input::processEvent(const SDL_Event* event) {
    // Only detect mouse movements, not button events
    switch (event->type) {
        case SDL_MOUSEMOTION:
            mousePosition_.x = static_cast<float>(event->motion.x);
            mousePosition_.y = static_cast<float>(event->motion.y);
            break;

        case SDL_MOUSEWHEEL:
            mouseWheelY_ = event->wheel.y;
            break;
    }
}

void Input::update() {
    // Compute delta (difference between frames)
    mouseDelta_ = mousePosition_ - lastMousePosition_;
    lastMousePosition_ = mousePosition_;

    // Reset transient values
    mouseWheelY_ = 0;
}

bool Input::isKeyPressed(SDL_Scancode key) {
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    return state[key];
}

bool Input::isMouseButtonPressed(Uint8 button) {
    Uint32 state = SDL_GetMouseState(nullptr, nullptr);
    return (state & SDL_BUTTON(button)) != 0;
}

Vector2 Input::getMousePosition() { return mousePosition_; }

Vector2 Input::getMouseDelta() { return mouseDelta_; }

float Input::getMouseX() { return mousePosition_.x; }

float Input::getMouseY() { return mousePosition_.y; }

float Input::getMouseWheelY() {
    // reset to zero after use
    float y = mouseWheelY_;
    mouseWheelY_ = 0;
    return y;
}

}  // namespace Sokka
