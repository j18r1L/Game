#ifndef OPENGLTEXTURE_H
#define OPENGLTEXTURE_H

#include "HartEng/Renderer/Texture.h"

#include <glad/glad.h>

namespace HE
{
    class OpenGLTexture2D: public Texture2D
    {
    private:
        std::string m_Path; // путь можно убрать мб
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
        GLenum m_DataFormat;

    public:

        OpenGLTexture2D(const std::string& filepath);
        virtual ~OpenGLTexture2D();

        void Bind(uint32_t slot = 0) const override;
        void SetData(void* data, uint32_t size) const override;


        uint32_t GetWidth() const override
        {
            return m_Width;
        }
        uint32_t GetHeight() const override
        {
            return m_Height;
        }
        uint32_t GetRendererID() const override
        {
            return m_RendererID;
        }
    };

    class OpenGLTextureCube: public TextureCube
    {
    private:
        std::string m_Path; // путь можно убрать мб
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
        GLenum m_DataFormat;

    public:

        OpenGLTextureCube(const std::string& path);
        virtual ~OpenGLTextureCube();

        void Bind(uint32_t slot = 0) const override;
        void SetData(void* data, uint32_t size) const override;


        uint32_t GetWidth() const override
        {
            return m_Width;
        }
        uint32_t GetHeight() const override
        {
            return m_Height;
        }
        uint32_t GetRendererID() const override
        {
            return m_RendererID;
        }
    };
}
#endif // OPENGLTEXTURE_H
