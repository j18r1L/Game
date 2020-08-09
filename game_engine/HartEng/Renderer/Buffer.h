#ifndef BUFFER_H
#define BUFFER_H


#include "HartEng/pch.h"
#include "HartEng/Core.h"
#include "HartEng/Log.h"

#ifdef HE_PLATFORM_WINDOW
#include "HartEng/Platform/Direct3D/Direct3DBuffer.h"
#endif

namespace HE
{
    // Используется для того чтобы в конструктор BufferLayout дать ShaderDataType::Float3 например
    enum class ShaderDataType
    {
        None = 0,
        Float, 
        Float2,
        Float3,
        Float4, 
        Mat3,
        Mat4, 
        Int,
        Int2, 
        Int3, 
        Int4, 
        Bool
    };

    // Функция для определения размера типа, мб сделать статичные 4, а не sizeof(float)
    static uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:     return sizeof(float);
        case ShaderDataType::Float2:    return sizeof(float) * 2;
        case ShaderDataType::Float3:    return sizeof(float) * 3;
        case ShaderDataType::Float4:    return sizeof(float) * 4;
        case ShaderDataType::Mat3:      return sizeof(float) * 3 * 3;
        case ShaderDataType::Mat4:      return sizeof(float) * 4 * 4;
        case ShaderDataType::Int:       return sizeof(int);
        case ShaderDataType::Int2:      return sizeof(int) * 2;
        case ShaderDataType::Int3:      return sizeof(int) * 3;
        case ShaderDataType::Int4:      return sizeof(int) * 4;
        case ShaderDataType::Bool:      return 1;
        }
        HE_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    // Структура содержащая в себе название layout-a, сдвиг, размер, тип
    struct BufferElement
    {
        std::string Name;
        uint32_t Offset;
        uint32_t Size;
        ShaderDataType Type;
        bool Normalized;
       
        BufferElement() {}

        BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) :
            Name(name),
            Offset(0),
            Type(type),
            Size(ShaderDataTypeSize(type)),
            Normalized(normalized)
        {}

        uint32_t GetComponentCount() const
        {
            switch (Type)
            {
            case ShaderDataType::Float:     return 1;
            case ShaderDataType::Float2:    return 2;
            case ShaderDataType::Float3:    return 3;
            case ShaderDataType::Float4:    return 4;
            case ShaderDataType::Mat3:      return 3 * 3;
            case ShaderDataType::Mat4:      return 4 * 4;
            case ShaderDataType::Int:       return 1;
            case ShaderDataType::Int2:      return 2;
            case ShaderDataType::Int3:      return 3;
            case ShaderDataType::Int4:      return 4;
            case ShaderDataType::Bool:      return 1;
            }
            HE_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return 0;
        }

    };

    // Класс содержащий в себе вектор layout-ов
    class BufferLayout
    {
    private:
        
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;

        void CalculateOffsetsAndStride()
        {
            uint32_t offset = 0;
            m_Stride = 0;
            for (auto& element : m_Elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }
    public:
        BufferLayout() {}
        // initializer_list чтобы BufferLayout layout = { {Float3, name_1}, {Float4, name_2} };, а не std::vector<BufferLayout> = ...
        BufferLayout(const std::initializer_list<BufferElement>& elements):
            m_Elements(elements)
        {
            CalculateOffsetsAndStride();
        }
        inline const std::vector<BufferElement>& GetElements() const
        {
            return m_Elements;
        }

        inline uint32_t GetStride() const
        {
            return m_Stride;
        }

        // Чтобы делать:  for (const auto& element : layout)
        std::vector<BufferElement>::iterator begin()
        {
            return m_Elements.begin();
        }
        std::vector<BufferElement>::iterator end()
        {
            return m_Elements.end();
        }

        // Чтобы делать: for (const auto& element : m_VertexBuffer->GetLayout())
        std::vector<BufferElement>::const_iterator begin() const
        {
            return m_Elements.begin();
        }
        std::vector<BufferElement>::const_iterator end() const
        {
            return m_Elements.end();
        }

    };
    class VertexBuffer
    {
    private:

    public:
        virtual ~VertexBuffer() {}

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        virtual void SetLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& GetLayout() const = 0;

        static VertexBuffer* Create(float* vertices, uint32_t size);
    };

    class IndexBuffer
    {
    private:

    public:
        virtual ~IndexBuffer() {}

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        virtual uint32_t GetCount() const = 0;

        static IndexBuffer* Create(uint32_t* indices, uint32_t count);
    };
}

#endif // BUFFER_H

