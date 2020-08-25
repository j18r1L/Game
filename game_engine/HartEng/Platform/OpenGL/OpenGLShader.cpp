#include "OpenGLShader.h"


#include "HartEng/Log.h"
#include "HartEng/Core.h"

#include <glm/gtc/type_ptr.hpp>
#include <fstream>

namespace HE
{
    static GLenum ShaderTypeFromString(const std::string& type)
    {
        HE_PROFILE_FUNCTION();
        if (type == "vertex")
            return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel")
            return GL_FRAGMENT_SHADER;
        if (type == "geometry")
            return GL_GEOMETRY_SHADER;
        HE_CORE_ASSERT(false, "Unknown shader type: " + type);
        return 0;
    }

    static std::string StringFromShaderType(GLenum type)
    {
        HE_PROFILE_FUNCTION();
        if (type == GL_VERTEX_SHADER)
            return "Vertex";
        if (type == GL_FRAGMENT_SHADER)
            return "Fragment";
        if (type == GL_GEOMETRY_SHADER)
            return "Geometry";
        HE_CORE_ASSERT(false, "Unknown shader type");
        return "";
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc):
        m_Name(name)
    {
        HE_PROFILE_FUNCTION();
        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;
        Compile(sources);
    }

    OpenGLShader::OpenGLShader(const std::string& path)
    {
        HE_PROFILE_FUNCTION();
        std::string source = ReadFile(path);
        auto shaderSources = PreProcess(source);
        Compile(shaderSources);

        auto lastSlash = path.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = path.rfind('.');
        auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
        m_Name = path.substr(lastSlash, count);

    }

    OpenGLShader::~OpenGLShader()
    {
        HE_PROFILE_FUNCTION();
        glDeleteProgram(m_RendererID);
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        HE_PROFILE_FUNCTION();
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            if (size != -1)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
                in.close();
            }
            else
            {
                HE_CORE_ERROR("Could not read from file '{0}'", filepath);
            }
        }
        else
        {
            HE_CORE_ERROR("Could not open file '{0}'", filepath);
        }

        return result;
    }

    void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
    {
        HE_PROFILE_FUNCTION();
        m_RendererID = -1;
        std::vector<GLenum> glShaderIDs;
        GLuint program = glCreateProgram();
        for (auto& kv: shaderSources)
        {
            GLenum type = kv.first;
            const std::string& source = kv.second;

            // Create an empty vertex shader handle
            GLuint shader = glCreateShader(type);

            // Send the vertex shader source code to GL
            // Note that std::string's .c_str is NULL character terminated.
            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

            // Compile the vertex shader
            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if(isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                // The maxLength includes the NULL character
                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                // We don't need the shader anymore.
                glDeleteShader(shader);

                // Use the infoLog as you see fit.
                if (infoLog.size() != 0)
                    HE_CORE_ERROR("{0}", infoLog.data());
                else
                    HE_CORE_ERROR("Couldn't parse error message!");
                HE_CORE_ASSERT(false, StringFromShaderType(type) + " shader compilation failure!")

                // In this simple program, we'll just leave
                break;
            }
            glAttachShader(program, shader);
            glShaderIDs.push_back(shader);
        }



        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.


        // Link our program
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            if (maxLength != 0)
            {                
                glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
            }
            

            // We don't need the program anymore.
            glDeleteProgram(program);
            // Don't leak shaders either.

            for (auto id: glShaderIDs)
                glDeleteShader(id);

            // Use the infoLog as you see fit.
            if (infoLog.size() != 0)
                HE_CORE_ERROR("{0}", infoLog.data());
            else
                HE_CORE_ERROR("Couldn't parse error message!");
            HE_CORE_ASSERT(false, "Shader link failure!")


            // In this simple program, we'll just leave
            return;
        }

        // Always detach shaders after a successful link.
        for (auto id: glShaderIDs)
            glDeleteShader(id);
        m_RendererID = program;

    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& shaderSrc)
    {
        HE_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = shaderSrc.find(typeToken, 0);

        while(pos != std::string::npos)
        {
            size_t eol = shaderSrc.find_first_of("\r\n", pos);
            HE_CORE_ASSERT(eol != std::string::npos, "Syntax error!");
            size_t begin = pos + typeTokenLength + 1;
            std::string type = shaderSrc.substr(begin, eol - begin);
            HE_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specificator: " + type);

            size_t nextLinePos = shaderSrc.find_first_not_of("\r\n", eol);
            pos = shaderSrc.find(typeToken, nextLinePos);
            shaderSources[ShaderTypeFromString(type)] = shaderSrc.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? shaderSrc.size() - 1 : nextLinePos));
        }

        return shaderSources;
    }

    void OpenGLShader::Bind() const
    {
        HE_PROFILE_FUNCTION();

        glUseProgram(m_RendererID);
    }
    void OpenGLShader::UnBind() const
    {
        HE_PROFILE_FUNCTION();

        glUseProgram(0);
    }

    const std::string& OpenGLShader::GetName() const
    {
        return m_Name;
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) const
    {
        HE_PROFILE_FUNCTION();

        glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value) const
    {
        HE_PROFILE_FUNCTION();

        glUniformMatrix3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }
    void OpenGLShader::SetVec4(const std::string& name, const glm::vec4& value) const
    {
        HE_PROFILE_FUNCTION();

        glUniform4f(glGetUniformLocation(m_RendererID, name.c_str()), value.x, value.y, value.z, value.w);
    }
    void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& value) const
    {
        HE_PROFILE_FUNCTION();

        glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), value.x, value.y, value.z);
    }
    void OpenGLShader::SetVec2(const std::string& name, const glm::vec2& value) const
    {
        HE_PROFILE_FUNCTION();

        glUniform2f(glGetUniformLocation(m_RendererID, name.c_str()), value.x, value.y);
    }
    void OpenGLShader::SetFloat(const std::string& name, float value) const
    {
        HE_PROFILE_FUNCTION();

        glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
    }
    void OpenGLShader::SetInt(const std::string& name, int value) const
    {
        HE_PROFILE_FUNCTION();

        glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
    }
    void OpenGLShader::SetBool(const std::string& name, bool value) const
    {
        HE_PROFILE_FUNCTION();

        glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
    }

}
