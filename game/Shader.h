#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class Shader
{
public:
    GLuint ID;
    Shader(std::string vertexPath, std::string fragmentPath, std::string geometryPath = "");
    ~Shader() {}

    GLuint getProgram() {return ID;}
    void Use();
    void SetVec2(const std::string location, const glm::vec2& value) const;
    void SetVec3(const std::string location, const glm::vec3& value) const;
    void SetVec4(const std::string location, const glm::vec4& value) const;
    void SetFloat(const std::string location, const float& value) const;
    void SetMat4(const std::string location, const glm::mat4& value) const;
    void SetMat3(const std::string location, const glm::mat3& value) const;
    void SetInt(const std::string location, const int& value) const;

private:
    void checkCompileErrors(GLuint shader, std::string type) const;
};

#endif // SHADER_H
