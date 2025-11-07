#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>

namespace Sokka {

class Time {
public:
    Time();
    ~Time() = default;

    void update();
    void limitFPS();

    // getters
    int getFPS() const { return fps_; }
    float getDeltaTime() const { return deltaTime_; }
    Uint32 getTime() const { return currentTime_; }

private:
    Uint32 currentTime_;
    Uint32 lastSecond_;
    Uint32 lastFrame_;
    Uint32 frameTime_;
    float deltaTime_;
    int frames_;
    int fps_;

    static constexpr float MS_TO_SECONDS_F = 1000.0f;
    static constexpr Uint32 MS_TO_SECONDS = 1000;
    static constexpr int MAX_FPS = 240;
    static constexpr Uint32 TICK_PER_FRAME = MS_TO_SECONDS / MAX_FPS;
};

}  // namespace Sokka
