#include "Sandbox2D.h"

#include "../../Nut/vendor/glm/glm/gtc/type_ptr.hpp"

namespace Sandbox {
	void Sandbox2D::OnAttach()
	{
		m_CheckerboardTexture = Nut::Texture2D::Create("assets/textures/Checkerboard.png");
	}

	void Sandbox2D::OnUpdate(Nut::Timestep ts)
	{
		m_CameraController.OnUpdate(ts);

		Nut::RenderCommand::SetClearColor({ 0.3, 0.1, 0.5, 1.0 });
		Nut::RenderCommand::Clear();

		Nut::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Nut::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.3f }, m_Color);
		Nut::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, m_Color);

		Nut::Renderer2D::EndScene();
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

