#pragma once

#include <cmath>
namespace Sokka {

// Does not allow float values for now for pixel perfect display
struct Vector2 {
    float x;
    float y;

    Vector2() : x(0), y(0) {};
    Vector2(float x, float y) : x(x), y(y) {};

    Vector2 operator+(const Vector2& other) const { return {x + other.x, y + other.y}; }
    Vector2 operator-(const Vector2& other) const { return {x - other.x, y - other.y}; }
    Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
    Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }

    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    float magnitude() const { return std::sqrt(x * x + y * y); }
};

}  // namespace Sokka
