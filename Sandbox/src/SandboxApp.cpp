#include <Nut.h>
#include "../../Nut/vendor/glm/glm/gtc/type_ptr.hpp"
#include <Nut/Platform/OpenGL/OpenGLShader.h>

class ExampleLayer : public Nut::Layer {
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f) {
		m_VertexArray.reset(Nut::VertexArray::Create());

		float vertices[3 * 3 * 3 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.2f, 0.5f, 0.2f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.6f, 0.2f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.8f, 0.8f, 1.0f
		};

		m_VertexBuffer.reset(Nut::VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			Nut::BufferLayout layout = {
				{ Nut::ShaderDataType::Float3, "a_Pos" },
				{ Nut::ShaderDataType::Float4, "a_Clr" }
			};
			m_VertexBuffer->SetLayout(layout);
		}

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);


		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(Nut::IndexBuffer::Create(indices, 3));

		m_VertexArray->AddIndexBuffer(m_IndexBuffer);

		std::string vertSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Pos;
			layout(location = 1) in vec4 a_Clr;

			uniform mat4 u_VPM;
			uniform mat4 u_MLM;

			out vec3 v_Pos;
			out vec4 v_Color;

			void main() {
				v_Pos = a_Pos;
				v_Color = a_Clr;
				gl_Position = u_VPM * u_MLM * vec4(a_Pos + 0.5, 1.0);
			}
		)";

		std::string fragSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec3 u_Color;

			in vec3 v_Pos;
			in vec4 v_Color;

			void main() {
				color = vec4(v_Pos * 0.5 + 0.5, 1.0);
				color = v_Color;
				color = vec4(u_Color, 1.0);
			}
		)";

		m_Shader.reset(Nut::Shader::Create(vertSrc, fragSrc));
	}

	void OnUpdate(Nut::Timestep ts) override {
		//NT_TRACE("Deltatime: {0}s ({1})", ts.GetSeconds(), ts.GetMilliseconds());

		if (Nut::Input::IsKeyPressed(NT_KEY_LEFT)) {
			m_CameraPosition.x -= m_CameraMovementSpeed * ts;
		}
		else if (Nut::Input::IsKeyPressed(NT_KEY_RIGHT)) {
			m_CameraPosition.x += m_CameraMovementSpeed * ts;
		}
		if (Nut::Input::IsKeyPressed(NT_KEY_UP)) {
			m_CameraPosition.y += m_CameraMovementSpeed * ts;
		}
		else if (Nut::Input::IsKeyPressed(NT_KEY_DOWN)) {
			m_CameraPosition.y -= m_CameraMovementSpeed * ts;
		}

		if (Nut::Input::IsKeyPressed(NT_KEY_A)) {
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		}
		else if (Nut::Input::IsKeyPressed(NT_KEY_D)) {
			m_CameraRotation += m_CameraRotationSpeed * ts;
		}

		Nut::RenderCommand::SetClearColor({ 0.3, 0.1, 0.5, 1.0 });
		Nut::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		std::dynamic_pointer_cast<Nut::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_Color);

		Nut::Renderer::BeginScene(m_Camera);
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.22, y * 0.22, 0.0f);
				glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Nut::Renderer::Submit(m_Shader, m_VertexArray, transform);
			}
		}
		Nut::Renderer::EndScene();
	}

	void OnEvent(Nut::Event& e) override { return; }
	void OnAttach() override { return; }
	void OnDetach() override { return; }
	void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("TriangleColor", glm::value_ptr(m_Color));
		ImGui::End();
		return; 
	}
private:
	Nut::Ref<Nut::Shader> m_Shader;
	Nut::Ref<Nut::VertexBuffer> m_VertexBuffer;
	Nut::Ref<Nut::IndexBuffer> m_IndexBuffer;
	Nut::Ref<Nut::VertexArray> m_VertexArray;

	Nut::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;

	float m_CameraMovementSpeed = 1.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 5.0f;

	glm::vec3 m_Color;
};

class Sandbox : public Nut::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};


Nut::Application* Nut::CreateApplication() {
	return new Sandbox();
}
 