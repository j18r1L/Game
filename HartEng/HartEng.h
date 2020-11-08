#ifndef HARTENG_H
#define HARTENG_H

// Core
#include "HartEng/Core/Timestep.h"
#include "HartEng/Core/Core.h"
#include "HartEng/Core/Application.h"
#include "HartEng/Core/Log.h"
#include "HartEng/Core/Layer.h"
#include "HartEng/Core/Input.h"

// Scene
#include "HartEng/Scene/Scene.h"
#include "HartEng/Scene/Components/MaterialComponent.h"
#include "HartEng/Scene/Components/MeshComponent.h"
#include "HartEng/Scene/Components/SubMeshComponent.h"
#include "HartEng/Scene/Components/Texture2DComponent.h"
#include "HartEng/Scene/Components/TransformComponent.h"
#include "HartEng/Scene/Components/CameraComponent.h"
#include "HartEng/Scene/LoadMesh.h"


// Renderer
#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Renderer/RenderCommand.h"
#include "HartEng/Renderer/Shader.h"
#include "HartEng/Renderer/Buffer.h"
#include "HartEng/Renderer/VertexArray.h"
#include "HartEng/Renderer/Cameras.h"
#include "HartEng/Renderer/CameraControllers.h"
#include "HartEng/Renderer/Texture.h"
#include "HartEng/Renderer/Framebuffer.h"

// Events
#include "HartEng/Events/Event.h"
#include "HartEng/Events/KeyEvent.h"
#include "HartEng/Events/ApplicationEvent.h"
#include "HartEng/Events/MousesEvent.h"

// imGUI
#include "HartEng/imGUI/imGUILayer.h"


// Entry point
//#include "HartEng/EntryPoint.h"

// Key and Mouse Codes
#include "HartEng/Core/MouseButtonCodes.h"
#include "HartEng/Core/Keycodes.h"

#endif // HARTENG_H
