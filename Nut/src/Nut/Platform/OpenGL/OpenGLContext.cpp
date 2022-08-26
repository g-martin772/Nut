#include "ntpch.h"
#include "glad/glad.h"
#include "OpenGLContext.h"


namespace Nut {
	OpenGLContext::OpenGLContext(GLFWwindow* window) : m_WindowHandel(window) {
		NT_CORE_ASSERT(window, "WindowHandle is null!");
	}

	void OpenGLContext::Init()
	{
		NT_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandel);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		NT_CORE_ASSERT(status, "Failed to initialize GLAD");

		NT_CORE_INFO("OpenGL Renderer: {0} {1} Driver: {2}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
		int major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		NT_CORE_INFO("OpenGL Version: {0}.{1}", major, minor);
	}
	void OpenGLContext::SwapBuffers()
	{
		NT_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WindowHandel);
	}
}