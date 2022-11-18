#include "ntpch.h"
#include "Application.h"

#include <glm/glm.hpp>

#include "Nut/Core/Input.h"
#include "Nut/Core/Log.h"
#include "Nut/Events/AppEvent.h"
#include "Nut/Core/LayerStack.h"
#include "Nut/Renderer/Renderer.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define EVENTLOGGER 0

namespace Nut {
	Application* Application::s_Instance = nullptr;

	Application::Application(const char* name, uint32_t width, uint32_t height, ApplicationCommandLineArgs args) 
		: m_CommandLineArgs(args)
	{
		NT_PROFILE_FUNCTION();

		NT_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		WindowProps windowProperties;
		windowProperties.Title = name;
		windowProperties.Width = width;
		windowProperties.Height = height;
		m_Window = Scope<Window>(Window::Create(windowProperties));
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(false);

		Renderer::Init();

		{
			NT_PROFILE_FUNCTION();
			m_ImGuiLayer = new ImGuiLayer();
		}
		{
			NT_PROFILE_FUNCTION();
			PushOverlay(m_ImGuiLayer);
		}
	}

	Application::~Application() {

	}

	void Application::PushLayer(Layer* layer) {
		NT_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		NT_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		NT_PROFILE_FUNCTION();
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
		NT_PROFILE_FUNCTION();
		m_Running = false;
		return true;
	}

	bool Application::WindoResize(WindowResizeEvent& e) {
		NT_PROFILE_FUNCTION();
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e);

		return false;
	}

	void Application::Run() {
		NT_PROFILE_FUNCTION();
		while (m_Running) {
			float time = (float)glfwGetTime(); // TODO Move this to platform to remove dependency
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			{
				NT_PROFILE_SCOPE("RunLoop->LayerUpdates");
				if (!m_Minimized)
					for (Layer* layer : m_LayerStack) {
						NT_PROFILE_SCOPE("RunLoop->OnLayerUpdate");
						layer->OnUpdate(timestep);
					}
			}
			{
				NT_PROFILE_SCOPE("RunLoop->ImGuiLayerUpdates");
				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack) {
					NT_PROFILE_SCOPE("RunLoop->OnImGuiRender");
					layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}
			{
				NT_PROFILE_SCOPE("RunLoop->OnWindowUpdate");
				m_Window->OnUpdate();
			}
		}
	}
	void Application::Close()
	{
		m_Running = false;
	}
}
