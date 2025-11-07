#pragma once
#include <GL/glew.h>
#include <SDL_opengl.h>

#include <string>

#include "sokka/core/Base.hpp"

namespace Sokka {

struct TextureConfig {
    unsigned int width = 0;
    unsigned int height = 0;
    Color color = Color::White();
    bool generateMipmap = true;
};

class Texture2D {
public:
    Texture2D() = default;
    ~Texture2D();

    void close();

    void bind() const;
    int getWidth() const { return data_.width; }
    int getHeight() const { return data_.height; }
    GLuint getRendererID() const { return data_.id; }

    static Shared<Texture2D> create(const std::string& path);
    static Shared<Texture2D> create(const TextureConfig& config);

private:
    bool init(const std::string& path);
    bool init(const TextureConfig& config);
    struct TextureData {
        GLuint id = 0;
        // These are unsigned but must be int for stb_load()
        int width = 0;
        int height = 0;
        int channel = 0;

        // True if created with string
        bool generateMipmap = true;
    };

    TextureData data_;
};

}  // namespace Sokka
