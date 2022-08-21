#include "ntpch.h"
#include "Buffer.h"

#include "Nut/Renderer/Renderer.h"
#include "Nut/Platform/OpenGL/OpenGLBuffer.h"

namespace Nut{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL:
		{
			return new OpenGLVertexBuffer(vertices, size);
		}
		default:
			NT_WARN("No API Selected!");
			NT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			break;
		}
		return nullptr;
	}
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL:
		{
			return new OpenGLIndexBuffer(indices, size);
		}
		default:
			NT_WARN("No API Selected!");
			NT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			break;
		}
		return nullptr;
	}

	
}
