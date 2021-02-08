#pragma once

#include "HartEng/Core/pch.h"
#include "HartEng/Renderer/VertexArray.h"

#include <glad/glad.h>

namespace HE
{
    class OpenGLVertexArray: public VertexArray
    {
    private:
        uint32_t m_RendererID = 0;

        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
    public:
        OpenGLVertexArray(); 
        ~OpenGLVertexArray();

        void Bind() const override;
        void UnBind() const override;

        void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) override;
        void SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) override;

        const std::vector<std::shared_ptr<VertexBuffer> > & GetVertexBuffers() const override
        {
            return m_VertexBuffers;
        }
        const std::shared_ptr<IndexBuffer> & GetIndexBuffer() const override
        {
            return m_IndexBuffer;
        }

    };
    
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:     return GL_FLOAT;
        case ShaderDataType::Float2:    return GL_FLOAT;
        case ShaderDataType::Float3:    return GL_FLOAT;
        case ShaderDataType::Float4:    return GL_FLOAT;
        case ShaderDataType::Mat3:      return GL_FLOAT;
        case ShaderDataType::Mat4:      return GL_FLOAT;
        case ShaderDataType::Int:       return GL_INT;
        case ShaderDataType::Int2:      return GL_INT;
        case ShaderDataType::Int3:      return GL_INT;
        case ShaderDataType::Int4:      return GL_INT;
        case ShaderDataType::Bool:      return GL_BOOL;
        }
        HE_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }
}

