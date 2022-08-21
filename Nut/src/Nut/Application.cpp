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

		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 3 * 3 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.2f, 0.5f, 0.2f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.6f, 0.2f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.8f, 0.8f, 1.0f
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Pos" },
				{ ShaderDataType::Float4, "a_Clr" }
			};
			m_VertexBuffer->SetLayout(layout);
		}

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		

		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, 3));

		m_VertexArray->AddIndexBuffer(m_IndexBuffer);

		float vertices2[3 * 3 * 3 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.2f, 0.5f, 0.2f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.6f, 0.2f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.8f, 0.8f, 1.0f
		};

		m_SquareVA.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> squareVB = std::make_shared<VertexBuffer>(VertexBuffer::Create(vertices2));
		std::shared_ptr<VertexBuffer> squareVB = std::make_shared<VertexBuffer>();


		std::string vertSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Pos;
			layout(location = 1) in vec4 a_Clr;

			out vec3 v_Pos;
			out vec4 v_Color;

			void main() {
				v_Pos = a_Pos;
				v_Color = a_Clr;
				gl_Position = vec4(a_Pos + 0.5, 1.0);
			}
		)";

		std::string fragSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Pos;
			in vec4 v_Color;

			void main() {
				color = vec4(v_Pos * 0.5 + 0.5, 1.0);
				color = v_Color;
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
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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
