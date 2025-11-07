#pragma once

#include <memory>
namespace Sokka {
struct Color {
    float r, g, b, a;

    constexpr Color(float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f)
        : r(red), g(green), b(blue), a(alpha) {}

    static constexpr Color White() { return {1.0f, 1.0f, 1.0f, 1.0f}; }
    static constexpr Color Black() { return {0.0f, 0.0f, 0.0f, 1.0f}; }
    static constexpr Color Grey() { return {0.5f, 0.5f, 0.5f, 1.0f}; }
    static constexpr Color Red() { return {1.0f, 0.0f, 0.0f, 1.0f}; }
    static constexpr Color Green() { return {0.0f, 1.0f, 0.0f, 1.0f}; }
    static constexpr Color Blue() { return {0.0f, 0.0f, 1.0f, 1.0f}; }
};

template <typename T>
using Unique = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Unique<T> makeUnique(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Shared = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Shared<T> makeShared(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}
}  // namespace Sokka

// Add logger to all files with base
#include "sokka/core/Logger.hpp"
