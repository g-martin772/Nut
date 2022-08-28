#pragma once

#include <Nut.h>

#include "Panels/SceneHierachyPanel.h"

namespace Nut {
	class EditorLayer : public Layer {
	public:
		inline EditorLayer() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true) {}
		inline ~EditorLayer() {}

		void OnUpdate(Nut::Timestep ts) override;
		void OnEvent(Nut::Event& e) override;
		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;
	private:
		void CreateNewScene();
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(ButtonPressedEvent& e);
	private:
		ShaderLibrary m_Shader;
		OrthographicCameraController m_CameraController;
		Ref<FrameBuffer> m_FrameBuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;

		Entity m_HoveredEntity;

		EditorCamera m_EditorCamera;

		bool m_ViewportFocus = true;
		bool m_ViewportHover = true;

		int m_GuizmoType = -1;

		glm::vec2 m_ViewportSize;
		glm::vec2 m_ViewportBounds[2];

		//Panels
		SceneHierachyPanel m_SceneHierachyPanel;
	};																				// hi
}