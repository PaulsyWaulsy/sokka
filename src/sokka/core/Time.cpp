#include "sokka/core/Time.hpp"

namespace Sokka {

Time::Time()
    : currentTime_(SDL_GetTicks()),
      lastSecond_(SDL_GetTicks()),
      lastFrame_(SDL_GetTicks()),
      frameTime_(0.0),
      deltaTime_(0.0),
      frames_(0),
      fps_(0) {}

void Time::update() {
    // Calculate delta time to keep physics consistent
    currentTime_ = SDL_GetTicks();
    deltaTime_ = (currentTime_ - lastFrame_) / MS_TO_SECONDS_F;
    lastFrame_ = currentTime_;

    if (deltaTime_ < 0) deltaTime_ = 0;

    frames_++;
    if (currentTime_ - lastSecond_ >= MS_TO_SECONDS) {
        lastSecond_ = currentTime_;
        fps_ = frames_;
        frames_ = 0;
    }
}

void Time::limitFPS() {
    frameTime_ = SDL_GetTicks() - currentTime_;
    if (frameTime_ < TICK_PER_FRAME) SDL_Delay(TICK_PER_FRAME - frameTime_);
}

}  // namespace Sokka
