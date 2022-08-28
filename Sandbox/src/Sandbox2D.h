#pragma once

#include <Nut.h>
#include "ParticleSystem.h"

namespace Sandbox {
	class Sandbox2D : public Nut::Layer {
	public:
		inline Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true) {}
		inline ~Sandbox2D() {}

		void OnUpdate(Nut::Timestep ts) override;
		void OnEvent(Nut::Event& e) override;
		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;
	private:
		Nut::ShaderLibrary m_Shader;
		Nut::OrthographicCameraController m_CameraController;

		Nut::Ref<Nut::Texture2D> m_CheckerboardTexture;
		Nut::Ref<Nut::Texture2D> m_SpriteSheet;
		Nut::Ref<Nut::SubTexture2D> m_Subtexture1;
		Nut::Ref<Nut::SubTexture2D> m_Subtexture2;
		Nut::Ref<Nut::SubTexture2D> m_Subtexture3;

		ParticleProps m_Particle;
		ParticleSystem m_ParticleSystem;

		glm::vec4 m_Color = {1.0f, 1.0f, 1.0f, 1.0f};
		float r = 0.0f;

		std::unordered_map<char, Nut::Ref<Nut::SubTexture2D>> s_TextureMap;
	};
}