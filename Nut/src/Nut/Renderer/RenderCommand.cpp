#include "ntpch.h"
#include "RenderCommand.h"

#include "Nut/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Nut {
#ifdef NT_OPENGL
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
#else
#error "No Render API selected"
#endif

}