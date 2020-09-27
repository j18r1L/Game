

#include "HartEng/Core/pch.h"

#include "HartEng/Renderer/RenderCommand.h"
#include "HartEng/Platform/OpenGL/OpenGLRendererAPI.h"

namespace HE
{
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}
