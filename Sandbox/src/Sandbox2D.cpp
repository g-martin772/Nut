#include "Sandbox2D.h"

#include "../../Nut/vendor/glm/glm/gtc/type_ptr.hpp"

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

namespace Sandbox {


	static uint32_t TileMapLengh = 8;
	static const char* TileMap=
		"SSSSSSSS"
		"SSSSSSSS"
		"SSSSSSSS"
		"DDGDDGGG"
		"WWWDGWWW"
		"DGGGDGGD"
		"DGDDDGGG"
		"GGDGGGGD"
		"DGGDGGDD"
		;

	static const char* TreeMap =
		"00000000"
		"00000000"
		"00000000"
		"00000000"
		"00000000"
		"00000000"
		"00000000"
		"0T000000"
		"00000000"
		;

	void Sandbox2D::OnAttach()
	{
		NT_PROFILE_FUNCTION();
		m_CheckerboardTexture = Nut::Texture2D::Create("assets/textures/Checkerboard.png");
		m_SpriteSheet = Nut::Texture2D::Create("assets/textures/tilemap_packed.png");

		s_TextureMap['0'] = Nut::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 0 }, { 16, 16 });
		s_TextureMap['S'] = Nut::SubTexture2D::CreateFromCoords(m_SpriteSheet, {  9, 16 }, { 16, 16 });
		s_TextureMap['W'] = Nut::SubTexture2D::CreateFromCoords(m_SpriteSheet, {  9, 10 }, { 16, 16 });
		s_TextureMap['D'] = Nut::SubTexture2D::CreateFromCoords(m_SpriteSheet, {  1, 13 }, { 16, 16 });
		s_TextureMap['G'] = Nut::SubTexture2D::CreateFromCoords(m_SpriteSheet, {  1, 16 }, { 16, 16 });
		s_TextureMap['T'] = Nut::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 18,  4 }, { 16, 16 }, { 3, 3 });

		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 1.0f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 3.0f, 1.0f };
		m_Particle.Position = { 0.0f, 0.0f };
	}

	void Sandbox2D::OnUpdate(Nut::Timestep ts)
	{
		NT_TRACE("Frametime: {0}ms", ts * 1000.0f);

		NT_PROFILE_FUNCTION();

		m_CameraController.OnUpdate(ts);

		{
			NT_PROFILE_SCOPE("Sandbox::OnUpdate -> RenderCommand.Clear");
			Nut::RenderCommand::SetClearColor({ 0.3, 0.1, 0.5, 1.0 });
			Nut::RenderCommand::Clear();
		}

		{
			NT_PROFILE_SCOPE("Sandbox::OnUpdate -> Renderer.Draw");
			Nut::Renderer2D::ResetStats();
			Nut::Renderer2D::BeginScene(m_CameraController.GetCamera());


			Nut::Renderer2D::DrawQuad({ 0.5f, 1.5f }, { 0.8f, 1.3f }, m_Color);
			Nut::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, 45, m_CheckerboardTexture, m_Color, 10);
			Nut::Renderer2D::DrawQuad({ 20.0f, 20.0f, 0.0f }, { 1.0f, 1.0f }, m_SpriteSheet);

			
			Nut::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f }, r, m_CheckerboardTexture);

			for (float y = -10.0f; y < 10.0f; y += 0.25f) {
				for (float x = -10.0f; x < 10.0f; x += 0.25f)
				{
					glm::vec4 color = { ((x + 5.0f) / 10.0f), 0.4f, ((y + 5.0f) / 10.0f), 1.0f };
					Nut::Renderer2D::DrawQuad({ x, y }, { 0.20f, 0.20f }, color);
				}
			}


			for (uint32_t y = 0; y < strlen(TileMap) / TileMapLengh; y++)
			{
				for (uint32_t x = 0; x < TileMapLengh; x++)
				{
					char key = TileMap[x + y * TileMapLengh];
					if(key == '0'){}
						//Nut::Renderer2D::DrawQuad({ -20.0f + x, -20.0f - y, -1.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
					else {
						auto texture = s_TextureMap[key];
						Nut::Renderer2D::DrawQuad({ -20.0f + x, -20.0f - y, 0.0f }, { 1.0f, 1.0f }, texture);
					}
				}
			}

			for (uint32_t y = 0; y < strlen(TreeMap) / TileMapLengh; y++)
			{
				for (uint32_t x = 0; x < TileMapLengh; x++)
				{
					char key = TreeMap[x + y * TileMapLengh];
					if (key == '0'){}
						//Nut::Renderer2D::DrawQuad({ -20.0f + x, -20.0f - y, -1.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
					else {
						auto texture = s_TextureMap[key];
						Nut::Renderer2D::DrawQuad({ -20.0f + x, -20.0f - y, 1.0f }, { 3.0f, 3.0f }, texture);
					}
				}
			}

			Nut::Renderer2D::EndScene();

			if (Nut::Input::IsMouseButtonPressed(0))
			{
				r = r + 0.2f * ts.GetMilliseconds();
			}

			//ParticleTest
			if (Nut::Input::IsMouseButtonPressed(1))
			{
				auto x = Nut::Input::GetMouseX();
				auto y = Nut::Input::GetMouseY();
				auto width = Nut::Application::Get().GetWindow().GetWidth();
				auto height = Nut::Application::Get().GetWindow().GetHeight();

				auto bounds = m_CameraController.GetBounds();
				auto pos = m_CameraController.GetCamera().GetPosition();
				x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
				y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
				m_Particle.Position = { x + pos.x, y + pos.y };
				for (int i = 0; i < 5; i++)
					m_ParticleSystem.Emit(m_Particle);
			}

			m_ParticleSystem.OnUpdate(ts);
			m_ParticleSystem.OnRender(m_CameraController.GetCamera());
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

		ImGui::Text(("Drawcalls    " + std::to_string(Nut::Renderer2D::GetStats().DrawCalls)).c_str());
		ImGui::Text(("QuadCount    " + std::to_string(Nut::Renderer2D::GetStats().QuadCount)).c_str());
		ImGui::Text(("VertexCount  " + std::to_string(Nut::Renderer2D::GetStats().GetTotalVertexCount())).c_str());
		ImGui::Text(("IndexCount   " + std::to_string(Nut::Renderer2D::GetStats().GetTotalIndexCount())).c_str());

		ImGui::ColorEdit4("Birth Color", glm::value_ptr(m_Particle.ColorBegin));
		ImGui::ColorEdit4("Death Color", glm::value_ptr(m_Particle.ColorEnd));
		ImGui::DragFloat("Life Time", &m_Particle.LifeTime, 0.1f, 0.0f, 1000.0f);

		ImGui::End(); //Settings
	}

	void Sandbox2D::OnDetach()
	{

	}
}

