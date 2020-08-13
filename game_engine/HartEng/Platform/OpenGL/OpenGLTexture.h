#ifndef OPENGLTEXTURE_H
#define OPENGLTEXTURE_H

#include "HartEng/Renderer/Texture.h"

namespace HE
{
    class OpenGLTexture2D: public Texture2D
    {
    private:
        std::string m_Path; // путь можно убрать мб
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;

    public:
        OpenGLTexture2D(const std::string& path);
        virtual ~OpenGLTexture2D();

        uint32_t GetWidth() const override
        {
            return m_Width;
        }
        uint32_t GetHeight() const override
        {
            return m_Height;
        }

        void Bind(uint32_t slot = 0) const override;
    };
}
#endif // OPENGLTEXTURE_H
