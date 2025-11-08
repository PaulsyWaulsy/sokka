#pragma once

#include <glm/glm.hpp>
#include <string>

#include "sokka/core/Base.hpp"
namespace Sokka {

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    void bind();
    void unbind();
    bool init(const std::string& vertexPath, const std::string& fragmentPath);

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setFloat2(const std::string& name, const glm::vec2& value) const;
    void setFloat3(const std::string& name, const glm::vec3& value) const;
    void setFloat4(const std::string& name, const glm::vec4& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

    int getId() const { return id_; }

    static Shared<Shader> create(const std::string& vertexPath, const std::string& fragmentPath);

private:
    unsigned int id_;
};

}  // namespace Sokka
