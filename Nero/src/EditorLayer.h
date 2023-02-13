#pragma once

#include <Nut.h>

#include "Panels/SceneHierachyPanel.h"
#include "Panels/ContentBrowserPanel.h"

namespace Nut {
	class EditorLayer : public Layer {
	public:
		inline EditorLayer() : Layer("Sandbox2D") {}
		inline ~EditorLayer() {}

		void OnUpdate(Nut::Timestep ts) override;
		void OnEvent(Nut::Event& e) override;
		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;
	private:
		void NewScene();
		void CreateNewScene();
		bool OnKeyPressed(KeyPressedEvent& e);
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);
		void OnDuplicateEntity();

		void OnScenePlay();
		void OnSceneStop();

		bool OnMouseButtonPressed(ButtonPressedEvent& e);

		// UI Panels
		void UI_Toolbar();
	private:
		//Renderer stuff
		Ref<FrameBuffer> m_FrameBuffer;

		float m_ElapsedTime = 0.0f;
		int m_FrameCount = 0;

		//ECS
		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;
		std::filesystem::path m_EditorScenePath;
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