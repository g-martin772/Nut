#pragma once

#include <Nut.h>

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
		Nut::Ref<Nut::VertexBuffer> m_VertexBuffer;
		Nut::Ref<Nut::IndexBuffer> m_IndexBuffer;
		Nut::Ref<Nut::VertexArray> m_VertexArray;
		Nut::Ref<Nut::Texture2D> m_Texture;
		Nut::Ref<Nut::Texture2D> m_Texture2;
		Nut::OrthographicCameraController m_CameraController;

		glm::vec4 m_Color = {1.0f, 1.0f, 1.0f, 1.0f};
	};
}