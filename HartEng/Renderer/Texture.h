#ifndef TEXTURE_H
#define TEXTURE_H

#include "HartEng/Core/pch.h"


namespace HE
{
    class Texture
    {
    public:
        virtual ~Texture() = default;

        virtual void Bind(uint32_t slot = 0) const = 0;
        virtual void SetData(void* data, uint32_t size) const = 0;


        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual uint32_t GetRendererID() const = 0;
    };

    class Texture2D: public Texture
    {
    public:
        static std::shared_ptr<Texture2D> Create(const std::string& path);
    };
}

#endif // TEXTURE_H
