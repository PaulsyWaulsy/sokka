#pragma once
#include <GL/glew.h>

#include <string>

class Texture {
   public:
    Texture() = default;
    ~Texture();

    bool loadFromFile(const std::string& path);
    void bind() const;
    GLuint id() const { return textureId_; }

    int width() const { return width_; }
    int height() const { return height_; }

   private:
    GLuint textureId_ = 0;
    int width_ = 0, height_ = 0, channels_ = 0;
};
