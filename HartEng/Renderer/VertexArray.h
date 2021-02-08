#pragma once

#include "HartEng/Core/pch.h"

#include "HartEng/Renderer/Buffer.h"


namespace HE
{
    class VertexArray
    {
    private:

    public:
        virtual ~VertexArray() {}

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

        static std::shared_ptr<VertexArray> Create();
    };
}


