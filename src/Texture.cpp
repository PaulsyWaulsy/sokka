#include "Texture.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::~Texture() {
    if (textureId_ != 0) {
        glDeleteTextures(1, &textureId_);
    }
}

bool Texture::loadFromFile(const std::string& path) {
    unsigned char* data = stbi_load(path.c_str(), &width_, &height_, &channels_, STBI_rgb_alpha);
    if (!data) {
        std::cerr << "Failed to load image: " << path << " (" << stbi_failure_reason() << ")\n";
        return false;
    }

    glGenTextures(1, &textureId_);
    glBindTexture(GL_TEXTURE_2D, textureId_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Basic sampling parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(data);
    return true;
}

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, textureId_); }
