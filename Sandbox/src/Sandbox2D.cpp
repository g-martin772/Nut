#include "Sandbox2D.h"

#include "../../Nut/vendor/glm/glm/gtc/type_ptr.hpp"
#include <Nut/Platform/OpenGL/OpenGLShader.h>

namespace Sandbox {
	void Sandbox2D::OnAttach()
	{
		m_VertexArray.reset(Nut::VertexArray::Create());

		float vertices[] = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		m_VertexBuffer.reset(Nut::VertexBuffer::Create(vertices, sizeof(vertices)));

		Nut::BufferLayout layout = {
			{ Nut::ShaderDataType::Float3, "a_Pos" },
			{ Nut::ShaderDataType::Float2, "a_TexCoord" }
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);


		uint32_t indices[] = { 0, 1, 3, 2, 1, 3 };
		m_IndexBuffer.reset(Nut::IndexBuffer::Create(indices, 6));

		m_VertexArray->AddIndexBuffer(m_IndexBuffer);
		m_Shader.Load("assets/shaders/Flat.glsl");

		m_Shader.Load("assets/shaders/Texture.glsl");
		m_Texture = (Nut::Texture2D::Create("assets/textures/Checkerboard.png"));
		m_Texture2 = (Nut::Texture2D::Create("assets/textures/ChernoLogo.png"));
		std::dynamic_pointer_cast<Nut::OpenGLShader>(m_Shader.Get("Texture"))->Bind();
		std::dynamic_pointer_cast<Nut::OpenGLShader>(m_Shader.Get("Texture"))->UploadUniformInt("u_Texture", 0);
	}

	void Sandbox2D::OnUpdate(Nut::Timestep ts)
	{
		m_CameraController.OnUpdate(ts);

		Nut::RenderCommand::SetClearColor({ 0.3, 0.1, 0.5, 1.0 });
		Nut::RenderCommand::Clear();

		std::dynamic_pointer_cast<Nut::OpenGLShader>(m_Shader.Get("Flat"))->Bind();
		std::dynamic_pointer_cast<Nut::OpenGLShader>(m_Shader.Get("Flat"))->UploadUniformFloat4("u_Color", m_Color);

		Nut::Renderer::BeginScene(m_CameraController.GetCamera());

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.22, y * 0.22, 0.0f);
				glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Nut::Renderer::Submit(m_Shader.Get("Flat"), m_VertexArray, transform);
			}
		}

		m_Texture->Bind();
		Nut::Renderer::Submit(m_Shader.Get("Texture"), m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_Texture2->Bind();
		Nut::Renderer::Submit(m_Shader.Get("Texture"), m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Nut::Renderer::EndScene();
	}

	void Sandbox2D::OnEvent(Nut::Event& e)
	{
		m_CameraController.OnEvent(e);
	}


	void Sandbox2D::OnImGuiRender()
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("RasterColor", glm::value_ptr(m_Color));
		ImGui::End();
		return;
	}

	void Sandbox2D::OnDetach()
	{

	}
}

