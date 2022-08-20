#pragma once

#include "Nut/Renderer/GraphicsContext.h"
#include "GLFW/glfw3.h"

namespace Nut {
	class OpenGLContext : public GraphicsContext{
	public:
		OpenGLContext(GLFWwindow* window);

		void Init() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandel;
	};
}
