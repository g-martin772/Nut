#include "ntpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Nut/Platform/OpenGL/OpenGLVertexArray.h"

Nut::VertexArray* Nut::VertexArray::Create()
{
	switch (Renderer::GetAPI()) {
	case RendererAPI::API::OpenGL:	return new OpenGLVertexArray();
	}
	NT_CORE_ASSERT(false, "RendererAPI::selected is currently not supported!")
	return nullptr;
}
