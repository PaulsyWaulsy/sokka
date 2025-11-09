#pragma once
#include <string>

#include "sokka/graphics/Vector2.hpp"

namespace Sokka {

class Canvas;

class Tool {
public:
    virtual ~Tool() = default;
    virtual void onMouseDown(const Vector2&) {}
    virtual void onMouseUp(const Vector2&) {}
    virtual void onMouseMove(const Vector2&) {}

    virtual void render() {}
    virtual const std::string& getName() const { return name_; }

protected:
    Tool(Canvas& canvas, const std::string& name) : canvas_(canvas), name_(name) {}

    Canvas& canvas_;
    std::string name_;
};

}  // namespace Sokka
