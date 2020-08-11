#ifndef SHADER_H
#define SHADER_H

#include "HartEng/pch.h"
#include "HartEng/Log.h"
#include <glm/glm.hpp>

namespace HE
{
    class Shader
    {
    private:
        uint32_t m_RendererID;
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();

        void Bind() const;
        void UnBind() const;

        void SetMat4(const std::string& name, const glm::mat4& matrix);

    };
}

#endif // SHADER_H
