
#pragma once

#include <string>
namespace Sokka {
class UIPanel {
public:
    UIPanel(const std::string& name = "Panel") : name_(name) {}
    virtual ~UIPanel() = default;

    virtual void init() {}
    virtual void render() = 0;

    const std::string& getName() const { return name_; }

protected:
    std::string name_;
};

}  // namespace Sokka
