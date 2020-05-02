#include "Shader.h"

Shader::Shader(std::string vertexPath, std::string fragmentPath, std::string geometryPath)
{
    bool IsGeometry = false;
    if (geometryPath.size() != 0)
        IsGeometry = true;
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        if(IsGeometry)
        {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    unsigned int geometry;
    if(IsGeometry)
    {
        const char * gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if(IsGeometry)
        glAttachShader(ID, geometry);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if(IsGeometry)
        glDeleteShader(geometry);

}

void Shader::Use()
{
    glUseProgram(ID);
}

void Shader::SetVec2(const std::string location, const glm::vec2& value) const
{
    glUniform2f(glGetUniformLocation(ID, location.c_str()), value.x, value.y);
}

void Shader::SetVec3(const std::string location, const glm::vec3& value) const
{
    glUniform3f(glGetUniformLocation(ID, location.c_str()), value.x, value.y, value.z);
}

void Shader::SetVec4(const std::string location, const glm::vec4& value) const
{
    glUniform4f(glGetUniformLocation(ID, location.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::SetMat4(const std::string location, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, location.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat3(const std::string location, const glm::mat3& value) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, location.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetFloat(const std::string location, const float& value) const
{
    glUniform1f(glGetUniformLocation(ID, location.c_str()), value);
}

void Shader::SetInt(const std::string location, const int& value) const
{
    glUniform1i(glGetUniformLocation(ID, location.c_str()), value);
}

void Shader::checkCompileErrors(GLuint shader, std::string type) const
{
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
