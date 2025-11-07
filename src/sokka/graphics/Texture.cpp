#include "sokka/graphics/Texture.hpp"

#include <vector>

#include "sokka/core/Base.hpp"
#include "sokka/core/Logger.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Sokka {

Texture2D::~Texture2D() {
    // close Texture
    close();
}

bool Texture2D::init(const std::string& path) {
    glGenTextures(1, &data_.id);
    glBindTexture(GL_TEXTURE_2D, data_.id);

    // Basic sampling parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    unsigned char* data =
        stbi_load(path.c_str(), &data_.width, &data_.height, &data_.channel, STBI_rgb_alpha);

    if (!data) {
        SOKKA_ERROR("Failed to load image: ", path, " (", stbi_failure_reason(), ")");
        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data_.width, data_.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 data);

    if (data_.generateMipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);
    return true;
}

bool Texture2D::init(const TextureConfig& config) {
    if (config.width == 0 || config.height == 0) {
        SOKKA_ERROR("TextureConfig dimensions must be non-zero: (", config.width, ", ",
                    config.height, ")");
        return false;
    }

    glGenTextures(1, &data_.id);
    glBindTexture(GL_TEXTURE_2D, data_.id);

    // Basic parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Solid-color fill
    uint8_t rgba[4] = {
        static_cast<uint8_t>(config.color.r * 255), static_cast<uint8_t>(config.color.g * 255),
        static_cast<uint8_t>(config.color.b * 255), static_cast<uint8_t>(config.color.a * 255)};

    std::vector<uint8_t> buffer(config.width * config.height * 4);
    for (size_t i = 0; i < buffer.size(); i += 4) memcpy(&buffer[i], rgba, 4);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, config.width, config.height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, buffer.data());

    if (config.generateMipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    return true;
}

void Texture2D::close() {
    if (data_.id != 0) {
        SOKKA_INFO("Texture deleted, ID = ", data_.id);
        glDeleteTextures(1, &data_.id);
    }
}

void Texture2D::bind() const { glBindTexture(GL_TEXTURE_2D, data_.id); }

Shared<Texture2D> Texture2D::create(const std::string& path) {
    auto texture = makeShared<Texture2D>();
    if (!texture->init(path)) {
        SOKKA_ERROR("Unable to initialise Texture");
        return nullptr;
    }
    return texture;
}

Shared<Texture2D> Texture2D::create(const TextureConfig& config) {
    auto texture = makeShared<Texture2D>();
    if (!texture->init(config)) {
        SOKKA_ERROR("Unable to initialise Texture");
        return nullptr;
    }
    return texture;
}

}  // namespace Sokka
