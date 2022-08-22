#include <Nut.h>

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

			out vec3 v_Pos;
			out vec4 v_Color;

			void main() {
				v_Pos = a_Pos;
				v_Color = a_Clr;
				gl_Position = u_VPM * vec4(a_Pos + 0.5, 1.0);
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

		m_Shader.reset(new Nut::Shader(vertSrc, fragSrc));
	}

	void OnUpdate() override {
		if (Nut::Input::IsKeyPressed(NT_KEY_LEFT)) {
			m_CameraPosition.x -= m_CameraMovementSpeed;
		}
		else if (Nut::Input::IsKeyPressed(NT_KEY_RIGHT)) {
			m_CameraPosition.x += m_CameraMovementSpeed;
		}
		if (Nut::Input::IsKeyPressed(NT_KEY_UP)) {
			m_CameraPosition.y += m_CameraMovementSpeed;
		}
		else if (Nut::Input::IsKeyPressed(NT_KEY_DOWN)) {
			m_CameraPosition.y -= m_CameraMovementSpeed;
		}

		if (Nut::Input::IsKeyPressed(NT_KEY_A)) {
			m_CameraRotation += m_CameraRotationSpeed;
		}
		else if (Nut::Input::IsKeyPressed(NT_KEY_D)) {
			m_CameraRotation -= m_CameraRotationSpeed;
		}

		Nut::RenderCommand::SetClearColor({ 0.3, 0.1, 0.5, 1.0 });
		Nut::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Nut::Renderer::BeginScene(m_Camera);
		Nut::Renderer::Submit(m_Shader, m_VertexArray);
		Nut::Renderer::EndScene();
	}

	void OnEvent(Nut::Event& e) override {
		
	}



	void OnAttach() override { return; }
	void OnDetach() override { return; }
	void OnImGuiRender() override { return; }
private:
	std::shared_ptr<Nut::Shader> m_Shader;
	std::shared_ptr<Nut::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Nut::IndexBuffer> m_IndexBuffer;
	std::shared_ptr<Nut::VertexArray> m_VertexArray;

	Nut::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;

	float m_CameraMovementSpeed = 0.1f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 2.0f;
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
 