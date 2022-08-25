#include "ntpch.h"
#include "Texture.h"

#include <Nut/Renderer/Renderer.h> 
#include <Nut/Platform/OpenGL/OpenGLTexture.h>

namespace Nut {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLTexture2D>(width, height);
		}
		default:
			NT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			break;
		}
		NT_CORE_ASSERT(false, "No API Selected!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLTexture2D>(path);
		}
		default:
			NT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			break;
		}
		NT_CORE_ASSERT(false, "No API Selected!");
		return nullptr;
	}
}

