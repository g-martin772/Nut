#include "ntpch.h"
#include "Application.h"

#include "Events/AppEvent.h"
#include "Log.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define EVENTLOGGER 0

namespace Nut {
	Application* Application::s_Instance = nullptr;

	Application::Application() {
		NT_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		unsigned int id;
		glGenVertexArrays(1, &id);
	}

	Application::~Application() {
		
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::WindoClose));
		#if EVENTLOGGER
		NT_CORE_TRACE("{0}", e);
		#endif

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled())
				break;
		}
	}

	bool Application::WindoClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	void Application::Run() {
		while (m_Running) {
			glClearColor(1, 1, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();
		}
	}
}
