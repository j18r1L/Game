#include "HartEng/pch.h"
#include "HartEng/Renderer/RendererAPI.h"

namespace HE
{
    // TODO сделать динамический выбор API
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

}
