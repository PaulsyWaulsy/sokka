#pragma once

#include "GL/glew.h"
#include "sokka/core/Base.hpp"

namespace Sokka {

struct FrameBufferCofig {
    int width, height;
    // Bossibly aditional settings
};

class FrameBuffer {
public:
    FrameBuffer(const FrameBufferCofig& config);
    ~FrameBuffer() = default;

    void bind();
    void unbind();

    void resize(int width, int height);

    const FrameBufferCofig& getConfig() const { return config_; }

    GLuint getColorAttachment() const { return colorTexture_; }

    static Shared<FrameBuffer> create(int width, int height);
    static Shared<FrameBuffer> create(const FrameBufferCofig& config);

private:
    bool init(const FrameBufferCofig& config);

    FrameBufferCofig config_;

    //
    GLuint fbo_ = 0;
    GLuint rbo_ = 0;
    GLuint colorTexture_ = 0;
};

}  // namespace Sokka
