#pragma once

#include "ntpch.h"
#include "Nut/Core.h"
#include "Nut/Window.h"
#include "Nut/Events/AppEvent.h"
#include "Nut/LayerStack.h"

namespace Nut {
	class NUT_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		bool WindoClose(WindowCloseEvent& e);
		LayerStack m_LayerStack;
		static Application* s_Instance;
	};


	//To be defined by client!
	Application* CreateApplication();
}

