#include "Sandbox2D.h"

#include "../../Nut/vendor/glm/glm/gtc/type_ptr.hpp"

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

namespace Sandbox {

	void Sandbox2D::OnAttach()
	{
		NT_PROFILE_FUNCTION();
		m_CheckerboardTexture = Nut::Texture2D::Create("assets/textures/Checkerboard.png");
	}

	void Sandbox2D::OnUpdate(Nut::Timestep ts)
	{
		NT_PROFILE_FUNCTION();

		m_CameraController.OnUpdate(ts);
		
		{
			NT_PROFILE_SCOPE("Sandbox::OnUpdate -> RenderCommand.Clear");
			Nut::RenderCommand::SetClearColor({ 0.3, 0.1, 0.5, 1.0 });
			Nut::RenderCommand::Clear();
		}
		
		{
			NT_PROFILE_SCOPE("Sandbox::OnUpdate -> Renderer.Draw");
			Nut::Renderer2D::BeginScene(m_CameraController.GetCamera());

			Nut::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.3f }, m_Color);
			Nut::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, glm::radians(45.0f), m_CheckerboardTexture, m_Color, 10);

			Nut::Renderer2D::EndScene(); 
		}
	}

	void Sandbox2D::OnEvent(Nut::Event& e)
	{
		m_CameraController.OnEvent(e);
	}


	void Sandbox2D::OnImGuiRender()
	{
		NT_PROFILE_FUNCTION();
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("RasterColor", glm::value_ptr(m_Color));
		ImGui::End();
	}

	void Sandbox2D::OnDetach()
	{

	}
}

