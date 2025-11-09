#include "sokka/graphics/FrameBuffer.hpp"

#include "sokka/core/Base.hpp"
#include "sokka/core/Logger.hpp"

namespace Sokka {

FrameBuffer::FrameBuffer(const FrameBufferCofig& config) {
    if (!init(config)) {
        SOKKA_WARN("Failed to initialise frame buffer");
    }
}

bool FrameBuffer::init(const FrameBufferCofig& config) {
    if (config.width < 0 || config.height < 0) {
        SOKKA_ERROR("Frame buffer dimensions should not be negative");
        return false;
    }
    config_ = config;

    // --- Create framebuffer ---
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

    // color texture
    glGenTextures(1, &colorTexture_);
    glBindTexture(GL_TEXTURE_2D, colorTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, config.width, config.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture_, 0);

    // depth-stencil buffer
    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, config.width, config.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        SOKKA_ERROR("Framebuffer incomplete!");
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void FrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glViewport(0, 0, config_.width, config_.height);
}

void FrameBuffer::unbind() {
    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::resize(int width, int height) {
    config_.width = width;
    config_.height = height;

    glBindTexture(GL_TEXTURE_2D, colorTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

Shared<FrameBuffer> FrameBuffer::create(int width, int height) {
    return makeShared<FrameBuffer>(FrameBufferCofig{width, height});
}

Shared<FrameBuffer> FrameBuffer::create(const FrameBufferCofig& config) {
    return makeShared<FrameBuffer>(config);
}

}  // namespace Sokka
