#include "sokka/graphics/Shader.hpp"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "sokka/core/Base.hpp"
#include "sokka/core/Logger.hpp"

namespace Sokka {

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    if (!init(vertexPath, fragmentPath)) {
        SOKKA_ERROR("Failed to initialise Shader");
    }
    SOKKA_SUCCESS("Initialised Shader");
}

// TODO: refactor (Got reference code from LearnOpenGL)
bool Shader::init(const std::string& vertexPath, const std::string& fragmentPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure e) {
        SOKKA_ERROR("Failed to read shader file");
        return false;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        SOKKA_ERROR("Vertex shader compilation failed");
        return false;
    };

    // vertex Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        SOKKA_ERROR("Fragment shader compilation failed");
        return false;
    };

    // shader Program
    id_ = glCreateProgram();
    glAttachShader(id_, vertex);
    glAttachShader(id_, fragment);
    glLinkProgram(id_);
    // print linking errors if any
    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id_, 512, NULL, infoLog);
        SOKKA_ERROR("Shader program linking failed");
        return false;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return true;
}

void Shader::bind() { glUseProgram(id_); }

void Shader::unbind() { glUseProgram(0); }

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::setFloat2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(id_, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setFloat3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setFloat4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    GLint location = glGetUniformLocation(id_, name.c_str());
    if (location == -1) {
        SOKKA_WARN("Uniform: ", name, "not found or inactive");
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

Shared<Shader> Shader::create(const std::string& vertexPath, const std::string& fragmentPath) {
    return makeShared<Shader>(vertexPath, fragmentPath);
}

}  // namespace Sokka
