#include "HartEng/Platform/OpenGL/OpenGLVertexArray.h"

#include "HartEng/Core/Core.h"
#include "HartEng/Core/Log.h"
#include "HartEng/Renderer/Renderer.h"

#include <glad/glad.h>

namespace HE
{
    
    OpenGLVertexArray::OpenGLVertexArray()
    {
        HE_PROFILE_FUNCTION();
        Renderer::Submit([this]() mutable
            {
                glGenVertexArrays(1, &this->m_RendererID);
            });
    }
    OpenGLVertexArray::~OpenGLVertexArray()
    {
        HE_PROFILE_FUNCTION();

        GLuint rendererID = m_RendererID;
        Renderer::Submit([rendererID]()
            {
                glDeleteVertexArrays(1, &rendererID);
            });
    }

    void OpenGLVertexArray::Bind() const
    {
        HE_PROFILE_FUNCTION();
        GLuint rendererID = m_RendererID;
        Renderer::Submit([rendererID]()
            {
                glBindVertexArray(rendererID);
            });
        
    }
    void OpenGLVertexArray::UnBind() const
    {
        HE_PROFILE_FUNCTION();

        Renderer::Submit([]()
            {
                glBindVertexArray(0);
            });
        
    }

    void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer)
    {
        HE_PROFILE_FUNCTION();

        Renderer::Submit([this, vertexBuffer]() mutable
            {
                glBindVertexArray(m_RendererID);
                vertexBuffer->Bind();

                HE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no Layout!");

                uint32_t index = 0;
                const auto& layout_1 = vertexBuffer->GetLayout();
                for (const auto& element : layout_1)
                {
                    glEnableVertexAttribArray(index);
                    auto glBaseType = ShaderDataTypeToOpenGLBaseType(element.Type);
                    if (glBaseType == GL_INT)
                    {
                        glVertexAttribIPointer(index,
                            element.GetComponentCount(),
                            glBaseType,
                            layout_1.GetStride(),
                            (const void*)(intptr_t)element.Offset);
                    }
                    else
                    {
                        glVertexAttribPointer(index,
                            element.GetComponentCount(),
                            glBaseType,
                            element.Normalized ? GL_TRUE : GL_FALSE,
                            layout_1.GetStride(),
                            (const void*)(intptr_t)element.Offset);
                    }
                    index++;
                }
                m_VertexBuffers.push_back(vertexBuffer);
            });
        
    }

    void OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        HE_PROFILE_FUNCTION();

        Renderer::Submit([this, indexBuffer]() mutable
            {
                glBindVertexArray(m_RendererID);
                indexBuffer->Bind();
                m_IndexBuffer = indexBuffer;
            });
        
    }



}

