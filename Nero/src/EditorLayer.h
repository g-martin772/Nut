#pragma once

#include <Nut.h>

#include "Panels/SceneHierachyPanel.h"
#include "Panels/ContentBrowserPanel.h"

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
		void OpenScene(const std::filesystem::path& path);
		bool OnMouseButtonPressed(ButtonPressedEvent& e);

		void OnScenePlay();
		void OnSceneStop();

		// UI Panels
		void UI_Toolbar();
	private:
		//Renderer stuff
		ShaderLibrary m_Shader;
		OrthographicCameraController m_CameraController;
		Ref<FrameBuffer> m_FrameBuffer;

		//ECS
		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_HoveredEntity;
		EditorCamera m_EditorCamera;

		//Viewport stuff + ImGui/zmo
		bool m_ViewportFocus = true;
		bool m_ViewportHover = true;
		int m_GuizmoType = -1;
		glm::vec2 m_ViewportSize;
		glm::vec2 m_ViewportBounds[2];

		//Panels
		SceneHierachyPanel m_SceneHierachyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;


		//Scene State
		enum class SceneState
		{
			Edit = 0, Play = 1
		};
		SceneState m_SceneState = SceneState::Edit;

		//Editor resources
		Ref<Texture2D> m_IconPlay, m_IconStop;
	};																				// hi
}