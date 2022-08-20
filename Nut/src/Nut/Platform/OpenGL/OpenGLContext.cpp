#include "ntpch.h"
#include "glad/glad.h"
#include "OpenGLContext.h"


namespace Nut {
	OpenGLContext::OpenGLContext(GLFWwindow* window) : m_WindowHandel(window) {
		NT_CORE_ASSERT(window, "WindowHandle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandel);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		NT_CORE_ASSERT(status, "Failed to initialize GLAD");

		NT_CORE_INFO("OpenGL Renderer: {0} {1} Driver: {2}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandel);
	}
}