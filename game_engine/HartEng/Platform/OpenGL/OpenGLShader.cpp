#include "OpenGLShader.h"

#include "HartEng/pch.h"
#include "HartEng/Log.h"
#include "HartEng/Core.h"

#include <glm/gtc/type_ptr.hpp>

namespace HE
{
    OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        // Create an empty vertex shader handle
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Send the vertex shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const GLchar* source = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &source, 0);

        // Compile the vertex shader
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(vertexShader);

            // Use the infoLog as you see fit.

            HE_CORE_ERROR("{0}", infoLog.data());
            HE_CORE_ASSERT(false, "Vertex shader compilation failure!")

            // In this simple program, we'll just leave
            return;
        }

        // Create an empty fragment shader handle
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the fragment shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        source = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

        // Compile the fragment shader
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(fragmentShader);
            // Either of them. Don't leak shaders.
            glDeleteShader(vertexShader);


            // Use the infoLog as you see fit.
            HE_CORE_ERROR("{0}", infoLog.data());
            HE_CORE_ASSERT(false, "Fragment shader compilation failure!")


            // In this simple program, we'll just leave
            return;
        }

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        m_RendererID = glCreateProgram();

        // Attach our shaders to our program
        glAttachShader(m_RendererID, vertexShader);
        glAttachShader(m_RendererID, fragmentShader);

        // Link our program
        glLinkProgram(m_RendererID);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(m_RendererID);
            // Don't leak shaders either.
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            // Use the infoLog as you see fit.
            HE_CORE_ERROR("{0}", infoLog.data());
            HE_CORE_ASSERT(false, "Shader link failure!")


            // In this simple program, we'll just leave
            return;
        }

        // Always detach shaders after a successful link.
        glDetachShader(m_RendererID, vertexShader);
        glDetachShader(m_RendererID, fragmentShader);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_RendererID);
    }
    void OpenGLShader::UnBind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }
    void OpenGLShader::SetVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4f(glGetUniformLocation(m_RendererID, name.c_str()), value.x, value.y, value.z, value.w);
    }
    void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), value.x, value.y, value.z);
    }
    void OpenGLShader::SetVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2f(glGetUniformLocation(m_RendererID, name.c_str()), value.x, value.y);
    }
    void OpenGLShader::SetFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
    }
    void OpenGLShader::SetInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
    }
    void OpenGLShader::SetBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
    }

}
