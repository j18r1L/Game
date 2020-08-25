#include "HartEng/pch.h"
#include "OpenGLTexture.h"

#include "HartEng/Core.h"
#include "HartEng/Log.h"
#include "stb_image.h"

namespace HE
{
    OpenGLTexture2D::OpenGLTexture2D(const std::string& path):
        m_Path(path),
        m_DataFormat(0)
    {
        HE_PROFILE_FUNCTION();

        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = nullptr;
        {
            HE_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        }
        if (data)
        {
            HE_CORE_ERROR("Failed to load image from: " + path);
            return;
        }
            
        //HE_CORE_ASSERT(data, "Failed to load image from: " + path);
        GLenum internalFormat = 0;
        if (channels == 1)
        {
            internalFormat = GL_RED;
            m_DataFormat = GL_RED;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            m_DataFormat = GL_RGB;
        }
        else if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            m_DataFormat = GL_RGBA;
        }
        HE_CORE_ASSERT(internalFormat & m_DataFormat, "Format not supported!");

        {
            HE_PROFILE_SCOPE("OpenGL CreateTexture - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
            // TODO перевод uint int
            m_Width = width;
            m_Height = height;

            // TODO mipmap-ы
            glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
            glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

            // TODO кастомные параметры
            glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
        }

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        HE_PROFILE_FUNCTION();

        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size) const
    {
        HE_PROFILE_FUNCTION();

        HE_CORE_ASSERT(size == m_Width * m_Height * m_DataFormat, "Data must be entire texture!");
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        
        HE_PROFILE_FUNCTION();
        
        //4.1 version
        GLenum format = GL_TEXTURE0;
        switch (slot)
        {
            case (0):
                format = GL_TEXTURE0;
                break;
            case (1):
                format = GL_TEXTURE1;
                break;
            case (2):
                format = GL_TEXTURE2;
                break;
            case (3):
                format = GL_TEXTURE3;
                break;
            case (4):
                format = GL_TEXTURE4;
                break;
            case (5):
                format = GL_TEXTURE5;
                break;
            case (6):
                format = GL_TEXTURE6;
                break;
                HE_CORE_ERROR("More than 6 textures is not currently supported for OpenGL 4.4 or less");
        }
        glActiveTexture(format);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        
        //4.5 version
        //glBindTextureUnit(slot, m_RendererID);
    }
}
