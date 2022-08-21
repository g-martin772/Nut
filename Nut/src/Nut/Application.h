#pragma once

#include "Nut/ImGui/ImGuiLayer.h"
#include "Nut/Core.h"
#include "Nut/Window.h"
#include "Nut/Events/AppEvent.h"
#include "Nut/LayerStack.h"
#include "Nut/Renderer/Shader.h"
#include <Nut/Renderer/Buffer.h>

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

		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	private:
		ImGuiLayer* m_ImGuiLayer;
	};


	//To be defined by client!
	Application* CreateApplication();
}

