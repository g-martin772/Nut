#include "ntpch.h"
#include "Application.h"

#include "Nut/Events/AppEvent.h"
#include "Log.h"
#include "Input.h"
#include "glm/glm.hpp"
#include "Nut/Core/LayerStack.h"
#include "Nut/Renderer/Renderer.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define EVENTLOGGER 0

namespace Nut {
	Application* Application::s_Instance = nullptr;

	Application::Application() {
		NT_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = Scope<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(false);
		
		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
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
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::WindoResize));
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

	bool Application::WindoResize(WindowResizeEvent& e) {
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e);

		return false;
	}

	void Application::Run() {
		while (m_Running) {
			float time = (float)glfwGetTime(); // TODO Move this to platform to remove dependency
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if(!m_Minimized)
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
}
