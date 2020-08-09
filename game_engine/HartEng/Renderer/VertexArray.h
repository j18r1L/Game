#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H


#include "HartEng/pch.h"

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

        static VertexArray* Create();
    };
}

#endif // VERTEXARRAY_H

