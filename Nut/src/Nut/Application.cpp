#include "ntpch.h"
#include "glad/glad.h"
#include "Application.h"

#include "Events/AppEvent.h"
#include "Log.h"
#include "GLFW/glfw3.h"
#include "Input.h"
#include "glm/glm.hpp"
#include "Nut/LayerStack.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define EVENTLOGGER 0

namespace Nut {
	Application* Application::s_Instance = nullptr;

	Application::Application() {
		NT_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexArray);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		std::string vertSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Pos;

			out vec3 v_Pos;

			void main() {
				v_Pos = a_Pos;
				gl_Position = vec4(a_Pos + 0.5, 1.0);
			}
		)";

		std::string fragSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Pos;

			void main() {
				color = vec4(v_Pos * 0.5 + 0.5, 1.0);
			}
		)";

		m_Shader.reset(new Shader(vertSrc, fragSrc));
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

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
}
