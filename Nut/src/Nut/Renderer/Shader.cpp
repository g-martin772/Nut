#include "ntpch.h"
#include "Shader.h"

#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>
#include <Nut/Renderer/Renderer.h>
#include <Nut/Platform/OpenGL/OpenGLShader.h>


namespace Nut {
	Shader* Shader::Create(const std::string& vertexSrc, const std::string FragmentSrc)
	{
		switch(Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		{
			return new OpenGLShader(vertexSrc, FragmentSrc);
		}
		default:
			NT_WARN("No API Selected!");
			NT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			break;
		}
		return nullptr;
	}
}
