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
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			NT_CORE_ASSERT(index < Count, "Invalid command line args index");
			return Args[index];
		}
	};

	class NUT_API Application
	{
	public:
		Application(const char* name = "Nut Engine", uint32_t width = 1280, uint32_t height = 720, ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }

		void SubmitToMainThread(const std::function<void()>& function);
	private:
		Scope<Window> m_Window;
		ApplicationCommandLineArgs m_CommandLineArgs;
		bool m_Running = true;
		bool m_Minimized = false;
		bool WindoClose(WindowCloseEvent& e);
		bool WindoResize(WindowResizeEvent& e);
		LayerStack m_LayerStack;
		static Application* s_Instance;
		float m_LastFrameTime = 0.0f;

		void ExecuteMainThreadQueue();
		std::vector<std::function<void()>> m_MainThreadQueue;
		std::mutex m_MainThreadQueueMutex;
	private:
		ImGuiLayer* m_ImGuiLayer;
	};


	//To be defined by client!
	Application* CreateApplication(ApplicationCommandLineArgs args);
}

