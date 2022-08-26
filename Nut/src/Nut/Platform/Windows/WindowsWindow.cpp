#include "ntpch.h"
#include "WindowsWindow.h"
#include "Nut/Events/AppEvent.h"
#include "Nut/Events/MouseEvent.h"
#include "Nut/Events/KeyEvents.h"
#include "Nut/Platform/OpenGL/OpenGLContext.h"

namespace Nut {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int code, const char* message) {
		NT_CORE_ERROR("GLFW Error {0}: {1}", code, message);
	}

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props) {
		NT_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;


		NT_CORE_INFO("Creating Window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized) {
			NT_PROFILE_SCOPE("glfwInit");
			// TODO GLFW Terminate on system shutdown
			int success = glfwInit();
			NT_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		{
			NT_PROFILE_SCOPE("glfwCreateWindow");
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		}
		{
			NT_PROFILE_SCOPE("glfwCreateGLContext");
			m_Context = new OpenGLContext(m_Window);
			m_Context->Init();
		}
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		{
			//GLFW Callbacks
			NT_PROFILE_SCOPE("glfwSetCallbacks");

			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowResizeEvent event(width, height);
				data.EventCallback(event);
				data.Width = width; data.Height = height;
				});

			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
				});

			glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int character) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(character);
				data.EventCallback(event);
				});

			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
				}break;
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.EventCallback(event);
				}break;
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
				}break;
				default:
					NT_CORE_WARN("Invalid key action!");
					break;
				}
				});

			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
				case GLFW_PRESS: {
					ButtonPressedEvent event(button);
					data.EventCallback(event);
				}break;
				case GLFW_RELEASE: {
					ButtonReleasedEvent event(button);
					data.EventCallback(event);
				}break;
				case GLFW_REPEAT:
					break;
				default:
					NT_CORE_WARN("Invalid key action!");
					break;
				}
				});

			glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xO, double yO) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event((float)xO, (float)yO);
				data.EventCallback(event);
				});

			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event((float)x, (float)y);
				data.EventCallback(event);
				});
		}
	}

	void WindowsWindow::Shutdown() { 
		NT_PROFILE_FUNCTION();
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate() {
		NT_PROFILE_FUNCTION();
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled) {
		NT_PROFILE_FUNCTION();

		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync;
	}
}
