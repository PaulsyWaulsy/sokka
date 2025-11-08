#include "sokka/graphics/Renderer.hpp"

namespace Sokka {

void Renderer::init() {}
void Renderer::close() {}

void Renderer::startScene(const Camera& camera) {
    shader_->bind();
    shader_->setMat4("u_ViewProjection", camera_.getViewProjection());
}

}  // namespace Sokka
