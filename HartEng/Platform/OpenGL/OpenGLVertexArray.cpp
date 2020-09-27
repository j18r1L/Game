#include "HartEng/Platform/OpenGL/OpenGLVertexArray.h"
#include "HartEng/Core/Core.h"
#include "HartEng/Core/Log.h"

#include <glad/glad.h>

namespace HE
{
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
    OpenGLVertexArray::OpenGLVertexArray()
    {
        HE_PROFILE_FUNCTION();
        glGenVertexArrays(1, &m_RendererID);
        
    }
    OpenGLVertexArray::~OpenGLVertexArray()
    {
        HE_PROFILE_FUNCTION();

        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const
    {
        HE_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
    }
    void OpenGLVertexArray::UnBind() const
    {
        HE_PROFILE_FUNCTION();

        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer)
    {
        HE_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        HE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no Layout!");

        uint32_t index = 0;
        const auto& layout_1 = vertexBuffer->GetLayout();
        for (const auto& element : layout_1)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, element.GetComponentCount(), GL_FLOAT, element.Normalized ? GL_TRUE : GL_FALSE, layout_1.GetStride(), (void*)element.Offset);
            index++;
        }
        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        HE_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();
        m_IndexBuffer = indexBuffer;
    }



}
