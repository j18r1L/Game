#ifndef SHADER_H
#define SHADER_H

#include "HartEng/pch.h"
#include "HartEng/Log.h"

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

    };
}

#endif // SHADER_H
