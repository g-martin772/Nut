#pragma once

#include "Nut/ImGui/ImGuiLayer.h"
#include "Nut/Core/Core.h"
#include "Nut/Core/Window.h"
#include "Nut/Events/AppEvent.h"
#include "Nut/Core/LayerStack.h"
#include "Nut/Renderer/Shader.h"
#include "Nut/Renderer/Buffer.h"
#include "Nut/Renderer/VertexArray.h"
#include "Nut/Camera/OrthographicCamera.h"
#include "Nut/Core/Timestep.h"

namespace Nut {
	class NUT_API Application
	{
	public:
		Application(const char* name = "Nut Engine", uint32_t width = 1280, uint32_t height = 720);
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		Scope<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
		bool WindoClose(WindowCloseEvent& e);
		bool WindoResize(WindowResizeEvent& e);
		LayerStack m_LayerStack;
		static Application* s_Instance;
		float m_LastFrameTime = 0.0f;
	private:
		ImGuiLayer* m_ImGuiLayer;
	};


	//To be defined by client!
	Application* CreateApplication();
}

