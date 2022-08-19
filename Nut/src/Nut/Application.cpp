#include "ntpch.h"
#include "Application.h"

#include "Events/AppEvent.h"
#include "Log.h"
#include "../GLFW/include/GLFW/glfw3.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define EVENTLOGGER 1

namespace Nut {
	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application() {
		
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::WindoClose));
		#if EVENTLOGGER
		NT_CORE_TRACE("{0}", e);
		#endif
	}

	bool Application::WindoClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	void Application::Run() {
		while (m_Running) {
			glClearColor(1, 1, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}
