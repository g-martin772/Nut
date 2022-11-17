#include "EditorLayer.h"

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

namespace Nut {
	void EditorLayer::OnAttach()
	{
		NT_PROFILE_FUNCTION();

		FrameBufferSpecs fbSpec;
		fbSpec.Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth};
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(fbSpec);

		m_ActiveScene = std::make_shared<Scene>();
		m_SceneHierachyPanel.SetContext(m_ActiveScene);

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0

		m_SquareEntity = m_ActiveScene->CreateEntity("Square");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0, 1, 0, 1 });

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
		m_CameraEntity.AddComponent<CameraComponent>();

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
			}

			void OnDestroy()
			{
			}

			void OnUpdate(Timestep ts)
			{
				auto& transform = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;


				if (Input::IsKeyPressed(NT_KEY_A))
					transform.x -= speed * ts;
				if (Input::IsKeyPressed(NT_KEY_D))
					transform.x += speed * ts;
				if (Input::IsKeyPressed(NT_KEY_W))
					transform.y += speed * ts;
				if (Input::IsKeyPressed(NT_KEY_S))
					transform.y -= speed * ts;

			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

	}

	void EditorLayer::OnUpdate(Nut::Timestep ts)
	{
		NT_PROFILE_FUNCTION();
		if (FrameBufferSpecs spec = m_FrameBuffer->GetSpecs();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.Resize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			RenderCommand::SetViewport(0, 0, (uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		if (m_ViewportFocus || m_ViewportHover) {
			m_EditorCamera.OnUpdate(ts);
			m_CameraController.OnUpdate(ts);
		}

		Renderer2D::ResetStats();
		m_FrameBuffer->Bind();

		RenderCommand::SetClearColor({ 0.3, 0.1, 0.5, 1.0 });
		RenderCommand::Clear();

		m_FrameBuffer->ClearAttachment(1, -1);


		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

		/*Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (float y = -10.0f; y < 10.0f; y += 0.25f) {
			for (float x = -10.0f; x < 10.0f; x += 0.25f)
			{
				glm::vec4 color = { ((x + 5.0f) / 10.0f), 0.4f, ((y + 5.0f) / 10.0f), 1.0f };
				Nut::Renderer2D::DrawQuad({ x, y }, { 0.20f, 0.20f }, color);
			}
		}

		Renderer2D::DrawQuad({ 0, 0 }, { 1, 1 }, { 0.9f, 0.5f, 0.0f, 1.0f });

		Renderer2D::EndScene();*/

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}

		m_FrameBuffer->UnBind();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(NT_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<ButtonPressedEvent>(NT_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnMouseButtonPressed(ButtonPressedEvent& e)
	{
		if (e.GetButton() == NT_MOUSE_BUTTON_LEFT) {
			if (m_ViewportHover && !ImGuizmo::IsOver() && !Input::IsKeyPressed(NT_KEY_LEFT_ALT)) {
				m_SceneHierachyPanel.SetSelectedEntity(m_HoveredEntity);
				return true;
			}
		}

		return false;
	}


	void EditorLayer::OnImGuiRender()
	{
		NT_PROFILE_FUNCTION();

		// ---------------------------------
		//  Dockspace
		// ---------------------------------
#if 1
		static bool opt_fullscreen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		bool p_open = true;
		ImGui::Begin("DockSpace", &p_open, window_flags);

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();

		NT_ASSERT(io.ConfigFlags & ImGuiConfigFlags_DockingEnable, "Dockspace disabled via flag!")
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Application"))
			{
				// TODO Dialog for normal save
				if (ImGui::MenuItem("New...", "Ctrl+N")) { m_ActiveScene = std::make_shared<Scene>(); m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y); m_SceneHierachyPanel.SetContext(m_ActiveScene); CreateNewScene(); };
				if (ImGui::MenuItem("Open...", "Ctrl+O")) { m_ActiveScene = std::make_shared<Scene>(); m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y); m_SceneHierachyPanel.SetContext(m_ActiveScene); SceneSerializer s(m_ActiveScene); s.Deserialize(FileDialog::OpenFile("Nut Scene (*.nut)\0*.nut\0")); };
				if (ImGui::MenuItem("Save...", "Ctrl+S")) { SceneSerializer s(m_ActiveScene); };
				if (ImGui::MenuItem("SaveAs...", "Ctrl+Shift+S")) { SceneSerializer s(m_ActiveScene); s.Serialize(FileDialog::SaveFile("Nut Scene (*.nut)\0*.nut\0")); };
				if (ImGui::MenuItem("Close", "Shift+Esc")) Nut::Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
#endif

		// ---------------------------------
		//  Actual GUI inside the dockspace
		// ---------------------------------

#if 1
		ImGui::Begin("RenderStats");

		ImGui::Text(("Drawcalls    " + std::to_string(Nut::Renderer2D::GetStats().DrawCalls)).c_str());
		ImGui::Text(("QuadCount    " + std::to_string(Nut::Renderer2D::GetStats().QuadCount)).c_str());
		ImGui::Text(("VertexCount  " + std::to_string(Nut::Renderer2D::GetStats().GetTotalVertexCount())).c_str());
		ImGui::Text(("IndexCount   " + std::to_string(Nut::Renderer2D::GetStats().GetTotalIndexCount())).c_str());

		ImGui::End(); //Settings

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y }; //Includes tab bar

		m_ViewportFocus = ImGui::IsWindowFocused();
		m_ViewportHover = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->SetConsumeEvents(!m_ViewportFocus && !m_ViewportHover);

		ImVec2 vpPanelsize = ImGui::GetContentRegionAvail();

		m_ViewportSize.x = vpPanelsize.x;
		m_ViewportSize.y = vpPanelsize.y;


		ImGui::Image((void*)m_FrameBuffer->GetColorAttachmentRendererID(0), { m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

#endif

		// -------------
		//     Gizmos
		// -------------
#if 1 
		Entity selectedEntity = m_SceneHierachyPanel.GetSelectedEntity();
		if (selectedEntity && m_GuizmoType != -1) {
			//Cam
			const glm::mat4& camProj = m_EditorCamera.GetProjection();
			glm::mat4 camView = m_EditorCamera.GetViewMatrix();
			ImGuizmo::SetOrthographic(false);

			//Transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			//Pos
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			//Snapping
			bool snap = Input::IsKeyPressed(NT_KEY_LEFT_CONTROL);
			float snap_value = 0.5f;

			if (m_GuizmoType == ImGuizmo::OPERATION::ROTATE)
				snap_value = 45.0f;
			float snapValues[3] = { snap_value, snap_value, snap_value };

			//Manipulate
			ImGuizmo::Manipulate(glm::value_ptr(camView), glm::value_ptr(camProj),
				(ImGuizmo::OPERATION)m_GuizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			//Updating Values
			if (ImGuizmo::IsUsing()) {
				glm::vec3 translation = glm::vec3(0), rotation = glm::vec3(0), scale = glm::vec3(0);
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scalation = scale;
			}
		}
#endif

		ImGui::End(); //Viewport
		ImGui::PopStyleVar();

		m_SceneHierachyPanel.OnImGuiRender();

		ImGui::End(); //Dockspace
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::CreateNewScene() {
		m_SquareEntity = m_ActiveScene->CreateEntity("Square");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0, 1, 0, 1 });

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
		m_CameraEntity.AddComponent<CameraComponent>();

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
			}

			void OnDestroy()
			{
			}

			void OnUpdate(Timestep ts)
			{
				auto& transform = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				bool shift = (Input::IsKeyPressed(NT_KEY_LEFT_SHIFT) || Input::IsKeyPressed(NT_KEY_RIGHT_SHIFT));

				if (Input::IsKeyPressed(NT_KEY_A) && shift)
					transform.x -= speed * ts;
				if (Input::IsKeyPressed(NT_KEY_D) && shift)
					transform.x += speed * ts;
				if (Input::IsKeyPressed(NT_KEY_W) && shift)
					transform.y += speed * ts;
				if (Input::IsKeyPressed(NT_KEY_S) && shift)
					transform.y -= speed * ts;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = (Input::IsKeyPressed(NT_KEY_LEFT_CONTROL) || Input::IsKeyPressed(NT_KEY_RIGHT_CONTROL));
		bool shift = (Input::IsKeyPressed(NT_KEY_LEFT_SHIFT) || Input::IsKeyPressed(NT_KEY_RIGHT_SHIFT));

		switch (e.GetKeyCode())
		{
		case NT_KEY_S: {
			if (control && shift) {
				SceneSerializer s(m_ActiveScene);
				s.Serialize(FileDialog::SaveFile("Nut Scene (*.nut)\0*.nut\0"));
				return true;
			}

			if (control) {
				return true;
			}

			return false;
		}
		case NT_KEY_O: {
			if (control) {
				m_ActiveScene = std::make_shared<Scene>();
				m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				m_SceneHierachyPanel.SetContext(m_ActiveScene); SceneSerializer s(m_ActiveScene);
				s.Deserialize(FileDialog::OpenFile("Nut Scene (*.nut)\0*.nut\0"));
				return true;
			}

			return false;
		}
		case NT_KEY_N: {
			if (control) {
				m_ActiveScene = std::make_shared<Scene>();
				m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				m_SceneHierachyPanel.SetContext(m_ActiveScene);
				CreateNewScene();
				return true;
			}

			return false;
		}
		case NT_KEY_ESCAPE: {
			if (shift) {
				Nut::Application::Get().Close();
				return true;
			}

			return false;
		}

						  // Gizmos
		case NT_KEY_Q:
			m_GuizmoType = -1;
			break;
		case NT_KEY_W:
			m_GuizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case NT_KEY_E:
			m_GuizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		case NT_KEY_R:
			m_GuizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
		return false;
	}
}
