#include "HartEng/Core/pch.h"
#include "HartEng/Platform/OpenGL/OpenGLTexture.h"
#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Core/Core.h"
#include "HartEng/Core/Log.h"
#include "stb_image.h"

namespace HE
{
    OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath):
        m_Path(filepath),
        m_DataFormat(0)
    {
        HE_PROFILE_FUNCTION();

        int width, height, channels;
        stbi_set_flip_vertically_on_load(false);
        {
            HE_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
            stbi_uc* data;
            data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
            m_ImageData.Allocate(width * height * channels);
            m_ImageData.Write(data, width * height * channels);
            stbi_image_free(data);
            //m_ImageData.Data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
        }
        if (&m_ImageData.Data[0] == nullptr)
        {
            HE_CORE_ERROR("Failed to load image from: " + filepath);
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
        m_Loaded = true;
        m_Width = width;
        m_Height = height;
        {
            Renderer::Submit([this, internalFormat]() mutable
                {
                    HE_PROFILE_SCOPE("OpenGL CreateTexture - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
                    // TODO перевод uint int
                    
                    /*
                    // TODO mipmap-ы
                    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
                    glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

                    // TODO кастомные параметры
                    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
                    */
                    // OpenGL 4.1
                    glGenTextures(1, &m_RendererID);
                    glBindTexture(GL_TEXTURE_2D, m_RendererID);
                    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, &m_ImageData.Data[0]);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    m_ImageData.Data.clear();
                    //stbi_image_free(&m_ImageData.Data[0]);
                });
            
        }
        
        
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        HE_PROFILE_FUNCTION();

        Renderer::Submit([this]() mutable {
            glDeleteTextures(1, &m_RendererID);
            });
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size) const
    {
        HE_PROFILE_FUNCTION();

        HE_CORE_ASSERT(size == m_Width * m_Height * m_DataFormat, "Data must be entire texture!");
        Renderer::Submit([this, data, size]()
            {
                glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
            });
        
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        
        HE_PROFILE_FUNCTION();
        
        //4.1 version
        Renderer::Submit([this, slot]()
            {
                GLenum format = GL_TEXTURE0;
                switch (slot)
                {
                case 0: format = GL_TEXTURE0; break;
                case 1: format = GL_TEXTURE1; break;
                case 2: format = GL_TEXTURE2; break;
                case 3: format = GL_TEXTURE3; break;
                case 4: format = GL_TEXTURE4; break;
                case 5: format = GL_TEXTURE5; break;
                case 6: format = GL_TEXTURE6; break;
                case 7: format = GL_TEXTURE7; break;
                case 8: format = GL_TEXTURE8; break;
                case 9: format = GL_TEXTURE9; break;
                case 10: format = GL_TEXTURE10; break;
                case 11: format = GL_TEXTURE11; break;
                case 12: format = GL_TEXTURE12; break;
                case 13: format = GL_TEXTURE13; break;
                case 14: format = GL_TEXTURE14; break;
                case 15: format = GL_TEXTURE15; break;
                case 16: format = GL_TEXTURE16; break;
                case 17: format = GL_TEXTURE17; break;
                case 18: format = GL_TEXTURE18; break;
                case 19: format = GL_TEXTURE19; break;
                case 20: format = GL_TEXTURE20; break;
                case 21: format = GL_TEXTURE21; break;
                case 22: format = GL_TEXTURE22; break;
                case 23: format = GL_TEXTURE23; break;
                case 24: format = GL_TEXTURE24; break;
                case 25: format = GL_TEXTURE25; break;
                case 26: format = GL_TEXTURE26; break;
                case 27: format = GL_TEXTURE27; break;
                case 28: format = GL_TEXTURE28; break;
                case 29: format = GL_TEXTURE29; break;
                case 30: format = GL_TEXTURE30; break;
                case 31: format = GL_TEXTURE31; break;
                HE_CORE_ERROR("More than 31 textures is not currently supported for OpenGL 4.4 or less");
                }
                glActiveTexture(format);
                glBindTexture(GL_TEXTURE_2D, m_RendererID);
            });
        
        
        //4.5 version
        /*
        Renderer::Submit([this, slot]() {
            glBindTextureUnit(slot, m_RendererID);
            });
        */
    }



    /////////////////////////////////////// TEXTURECUBEMAP //////////////////////////////////////

    OpenGLTextureCube::OpenGLTextureCube(const std::string& path):
        m_Path(path)
    {
        HE_PROFILE_FUNCTION();

        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);

        std::vector<std::string> faces
        {
            "right.png",
            "left.png",
            "top.png",
            "bottom.png",
            "front.png",
            "back.png"
        };

        for (uint32_t i = 0; i < 6; i++)
        {
            {
                HE_PROFILE_SCOPE("stbi_load - OpenGLTextureCube::OpenGLTextureCube(const std::string&)");
                stbi_uc* data;
                data = stbi_load((path + faces[i]).c_str(), &width, &height, &channels, 0);
                m_ImageData[i].Allocate(width * height * channels);
                m_ImageData[i].Write(data, width * height * channels);
                stbi_image_free(data);
                
            }
            if (&m_ImageData[i].Data[0] == nullptr)
            {
                HE_CORE_ERROR("Failed to load image from: " + path);
                return;
            }

            m_Width[i] = width;
            m_Height[i] = height;
            GLenum internalFormat = 0;
            if (channels == 1)
            {
                m_InternalFormat[i] = GL_RED;
                m_DataFormat[i] = GL_RED;
            }
            else if (channels == 3)
            {
                m_InternalFormat[i] = GL_RGB8;
                m_DataFormat[i] = GL_RGB;
            }
            else if (channels == 4)
            {
                m_InternalFormat[i] = GL_RGBA8;
                m_DataFormat[i] = GL_RGBA;
            }
            HE_CORE_ASSERT(m_InternalFormat[i] & m_DataFormat[i], "Format not supported!");
        }

        // OpenGL 4.1
        Renderer::Submit([this, faces]() mutable
            {
                glGenTextures(1, &m_RendererID);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
                for (uint32_t i = 0; i < 6; i++)
                {


                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat[i], m_Width[i], m_Height[i], 0, m_DataFormat[i], GL_UNSIGNED_BYTE, &m_ImageData[i].Data[0]);
                    m_ImageData[i].Data.clear();
                    //stbi_image_free(&m_ImageData[i].Data[0]);
                }
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            });
    }

    OpenGLTextureCube::~OpenGLTextureCube()
    {
        HE_PROFILE_FUNCTION();

        Renderer::Submit([this]() mutable
            {
                glDeleteTextures(1, &m_RendererID);
            });
        
    }

    void OpenGLTextureCube::SetData(void* data, uint32_t size) const
    {
        HE_PROFILE_FUNCTION();

        HE_CORE_ASSERT(false, "Set CubeMap texture currently not supported!");
        //HE_CORE_ASSERT(size == m_Width * m_Height * m_DataFormat, "Data must be entire texture!");
        //glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTextureCube::Bind(uint32_t slot) const
    {

        HE_PROFILE_FUNCTION();

        Renderer::Submit([this, slot]()
            {
                //4.1 version
                GLenum format = GL_TEXTURE0;
                switch (slot)
                {
                case 0: format = GL_TEXTURE0; break;
                case 1: format = GL_TEXTURE1; break;
                case 2: format = GL_TEXTURE2; break;
                case 3: format = GL_TEXTURE3; break;
                case 4: format = GL_TEXTURE4; break;
                case 5: format = GL_TEXTURE5; break;
                case 6: format = GL_TEXTURE6; break;
                case 7: format = GL_TEXTURE7; break;
                case 8: format = GL_TEXTURE8; break;
                case 9: format = GL_TEXTURE9; break;
                case 10: format = GL_TEXTURE10; break;
                case 11: format = GL_TEXTURE11; break;
                case 12: format = GL_TEXTURE12; break;
                case 13: format = GL_TEXTURE13; break;
                case 14: format = GL_TEXTURE14; break;
                case 15: format = GL_TEXTURE15; break;
                case 16: format = GL_TEXTURE16; break;
                case 17: format = GL_TEXTURE17; break;
                case 18: format = GL_TEXTURE18; break;
                case 19: format = GL_TEXTURE19; break;
                case 20: format = GL_TEXTURE20; break;
                case 21: format = GL_TEXTURE21; break;
                case 22: format = GL_TEXTURE22; break;
                case 23: format = GL_TEXTURE23; break;
                case 24: format = GL_TEXTURE24; break;
                case 25: format = GL_TEXTURE25; break;
                case 26: format = GL_TEXTURE26; break;
                case 27: format = GL_TEXTURE27; break;
                case 28: format = GL_TEXTURE28; break;
                case 29: format = GL_TEXTURE29; break;
                case 30: format = GL_TEXTURE30; break;
                case 31: format = GL_TEXTURE31; break;
                HE_CORE_ERROR("More than 31 textures is not currently supported for OpenGL 4.4 or less");
                }
                glActiveTexture(format);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
            });
        

        //4.5 version
        //glBindTextureUnit(slot, m_RendererID);
    }
}
