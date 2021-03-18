#pragma once

#include "HartEng/Core/pch.h"
#include "HartEng/Asset/Assets.h"


namespace HE
{
    class Texture : public Asset
    {
    public:
        virtual ~Texture() = default;

        virtual void Bind(uint32_t slot = 0) const = 0;
        virtual void SetData(void* data, uint32_t size) const = 0;
        virtual void SetFlipped(bool flipped) = 0;
        virtual bool Loaded() const = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual uint32_t GetRendererID() const = 0;
        virtual bool GetFlipped() const = 0;


    };

    class Texture2D: public Texture
    {
    public:
        virtual ~Texture2D() = default;
        static std::shared_ptr<Texture2D> Create(const std::string& filepath);
    };

    class TextureCube: public Texture
    {
    public:
        virtual ~TextureCube() = default;
        static std::shared_ptr<TextureCube> Create(const std::string& path);
    };
}