#include "EditorLayer.h"

#include <filesystem>
#include "Nut/Scripting/ScriptEngine.h"

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

namespace Nut {

	extern const std::filesystem::path g_AssetPath;

	void EditorLayer::OnAttach()
	{
		NT_PROFILE_FUNCTION();

		FrameBufferSpecs fbSpec;
		fbSpec.Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth};
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(fbSpec);

		m_EditorScene = std::make_shared<Scene>();
		m_ActiveScene = m_EditorScene;
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierachyPanel.SetContext(m_ActiveScene);

		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			NT_INFO("{0}", sceneFilePath);
			OpenScene(sceneFilePath);
		}

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		m_IconPlay = Texture2D::Create("resources/icons/editor/PlayButton.png");
		m_IconSimulate = Texture2D::Create("resources/icons/editor/SimulateButton.png");
		m_IconStop = Texture2D::Create("resources/icons/editor/StopButton.png");
		m_IconPause = Texture2D::Create("resources/icons/editor/PauseButton.png");
		m_IconStep = Texture2D::Create("resources/icons/editor/StepButton.png");
	}

	void EditorLayer::OnUpdate(Nut::Timestep ts)
	{
		NT_PROFILE_FUNCTION();

		m_ElapsedTime += ts.GetMilliseconds();
		if (m_ElapsedTime >= 1000) {
			NT_TRACE("Currently running with: {0}fps", m_FrameCount);
			m_FrameCount = 0;
			m_ElapsedTime = 0.0f;
		}
		m_FrameCount++;

		if (FrameBufferSpecs spec = m_FrameBuffer->GetSpecs();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			RenderCommand::SetViewport(0, 0, (uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		Renderer2D::ResetStats();
		m_FrameBuffer->Bind();

		RenderCommand::SetClearColor({ 0.3, 0.1, 0.5, 1.0 });
		RenderCommand::Clear();

		m_FrameBuffer->ClearAttachment(1, -1);

		switch (m_SceneState)
		{
			case SceneState::Edit:
			{
				m_EditorCamera.OnUpdate(ts);

				m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
				break;
			}
			case SceneState::Simulate:
			{
				m_EditorCamera.OnUpdate(ts);

				m_ActiveScene->OnUpdateSimulation(ts, m_EditorCamera);
				break;
			}
			case SceneState::Play:
			{
				m_ActiveScene->OnUpdateRuntime(ts);
				break;
			}
		}

		OnOverlayRender();

		//Mouse picking stuff
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
				if (ImGui::MenuItem("New...", "Ctrl+N")) { 
					m_ActiveScene = std::make_shared<Scene>(); 
					m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y); 
					m_SceneHierachyPanel.SetContext(m_ActiveScene);
					CreateNewScene(); 
					m_EditorScene = m_ActiveScene;
				};
				if (ImGui::MenuItem("Open...", "Ctrl+O")) { 
					m_ActiveScene = std::make_shared<Scene>(); 
					m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y); 
					m_SceneHierachyPanel.SetContext(m_ActiveScene); 
					SceneSerializer s(m_ActiveScene); 
					s.Deserialize(FileDialog::OpenFile("Nut Scene (*.nut)\0*.nut\0")); 
				};
				if (ImGui::MenuItem("Save...", "Ctrl+S")) { 
					SceneSerializer s(m_ActiveScene); 
				};
				if (ImGui::MenuItem("SaveAs...", "Ctrl+Shift+S")) { 
					SceneSerializer s(m_ActiveScene); 
					s.Serialize(FileDialog::SaveFile("Nut Scene (*.nut)\0*.nut\0")); 
				};
				if (ImGui::MenuItem("Close", "Shift+Esc")) 
					Nut::Application::Get().Close();

				ImGui::EndMenu();
			} 
			
			if (ImGui::BeginMenu("Script")) {
				if (ImGui::MenuItem("Reload Assambly", "Ctrl+R")) {
					ScriptEngine::ReloadAssembly();
				};

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View")) {
				if (ImGui::MenuItem("Toggle PhysicsCollider Visualization")) {
					m_ShowPhysicsColliders = !m_ShowPhysicsColliders;
				};

				ImGui::EndMenu();
			} 
			
			ImGui::EndMenuBar();
		}
#endif

		// ---------------------------------
		//  Actual GUI inside the Dockspace
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

		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHover);

		ImVec2 vpPanelsize = ImGui::GetContentRegionAvail();

		m_ViewportSize.x = vpPanelsize.x;
		m_ViewportSize.y = vpPanelsize.y;


		ImGui::Image((void*)m_FrameBuffer->GetColorAttachmentRendererID(0), { m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

#endif

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(g_AssetPath) / path);
			}
			ImGui::EndDragDropTarget();
		}



		// -------------
		//     Gizmos
		// -------------
#if 1 
		Entity selectedEntity = m_SceneHierachyPanel.GetSelectedEntity();
		if (selectedEntity && m_GuizmoType != -1 && m_SceneState == SceneState::Edit) {
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

		UI_Toolbar();

		m_SceneHierachyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();


		ImGui::End(); //Dockspace
	}

	void EditorLayer::OnOverlayRender()
	{
		if (m_SceneState == SceneState::Play)
		{
			Entity camera = m_ActiveScene->GetPrimaryCameraEntity();

			if (!camera)
				return;

			Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
		}
		else
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

		if (m_ShowPhysicsColliders)
		{
			// Box Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

					glm::vec3 translation = tc.Translation + glm::vec3(bc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scalation * glm::vec3(bc2d.Size * 2.0f, 1.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::scale(glm::mat4(1.0f), scale);

					RenderCommand::SetLineWidth(2.0f);
					Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));
				}
			}

			// Circle Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

					glm::vec3 translation = tc.Translation + glm::vec3(cc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scalation * glm::vec3(cc2d.Radius * 2.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::scale(glm::mat4(1.0f), scale);

					CircleRendererComponent collider(glm::vec4(0, 1, 0, 1));
					collider.Thickness = 0.02f;
					Renderer2D::DrawCircle(transform, collider, -1);
				}
			}
		}

		Renderer2D::EndScene();
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		bool toolbarEnabled = (bool)m_ActiveScene;

		ImVec4 tintColor = ImVec4(1, 1, 1, 1);
		if (!toolbarEnabled)
			tintColor.w = 0.5f;

		float size = ImGui::GetWindowHeight() - 4.0f;

		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		bool hasPlayButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play;
		bool hasSimulateButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate;
		bool hasPauseButton = m_SceneState != SceneState::Edit;

		

		if (hasPlayButton)
		{
			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate) ? m_IconPlay : m_IconStop;
			if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
					OnScenePlay();
				else if (m_SceneState == SceneState::Play)
					OnSceneStop();
			}
		}

		if (hasSimulateButton)
		{
			if (hasPlayButton)
				ImGui::SameLine();

			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? m_IconSimulate : m_IconStop;
			if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
					OnSceneSimulate();
				else if (m_SceneState == SceneState::Simulate)
					OnSceneStop();
			}
		}

		if (hasPauseButton)
		{
			bool isPaused = m_ActiveScene->IsPaused();
			ImGui::SameLine();
			{
				Ref<Texture2D> icon = m_IconPause;
				if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
				{
					m_ActiveScene->SetPaused(!isPaused);
				}
			}

			// Step button
			if (isPaused)
			{
				ImGui::SameLine();
				{
					Ref<Texture2D> icon = m_IconStep;
					bool isPaused = m_ActiveScene->IsPaused();
					if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
					{
						m_ActiveScene->Step();
					}
				}
			}
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
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
			if (control)
			{
				if (shift)
					SaveSceneAs();
				else
					SaveScene();
			}

			break;
		}
		case NT_KEY_O:
		{
			if (control)
				OpenScene();
			break;
		}
		case NT_KEY_N:
		{
			if (control)
				CreateNewScene();
			break;
		}
		case NT_KEY_ESCAPE: {
			if (shift) {
				Nut::Application::Get().Close();
				return true;
			}

			return false;
		}
		case NT_KEY_D:
		{
			if (control)
				OnDuplicateEntity();

			break;
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
			if (control)
				ScriptEngine::ReloadAssembly();
			else
				m_GuizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
		return false;
	}

	void EditorLayer::NewScene()
	{
		m_EditorScene = std::make_shared<Scene>();
		m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierachyPanel.SetContext(m_EditorScene);

		m_ActiveScene = m_EditorScene;

		m_EditorScenePath = std::filesystem::path();
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialog::OpenFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!filepath.empty())
			OpenScene(filepath);
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (m_SceneState != SceneState::Edit)
			OnSceneStop();

		if (path.extension().string() != ".nut")
		{
			NT_WARN("Could not load {0} - not a scene file", path.filename().string());
			return;
		}

		Ref<Scene> newScene = std::make_shared<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.Deserialize(path.string()))
		{
			m_EditorScene = newScene;
			m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierachyPanel.SetContext(m_EditorScene);

			m_ActiveScene = m_EditorScene;
			m_EditorScenePath = path;
		}
	}

	void EditorLayer::SaveScene()
	{
		if (!m_EditorScenePath.empty())
			SerializeScene(m_ActiveScene, m_EditorScenePath);
		else
			SaveSceneAs();
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialog::SaveFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
			SerializeScene(m_ActiveScene, filepath);
			m_EditorScenePath = filepath;
		}
	}

	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneSerializer serializer(scene);
		serializer.Serialize(path.string());
	}

	void EditorLayer::OnScenePlay()
	{
		if (m_SceneState == SceneState::Simulate)
			OnSceneStop();

		m_SceneState = SceneState::Play;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();

		m_SceneHierachyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneSimulate()
	{
		if (m_SceneState == SceneState::Play)
			OnSceneStop();

		m_SceneState = SceneState::Simulate;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnSimulationStart();

		m_SceneHierachyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneStop()
	{
		NT_CORE_ASSERT(m_SceneState == SceneState::Play || m_SceneState == SceneState::Simulate, "You cant stop the editor scene");

		if (m_SceneState == SceneState::Play)
			m_ActiveScene->OnRuntimeStop();
		else if (m_SceneState == SceneState::Simulate)
			m_ActiveScene->OnSimulationStop();

		m_SceneState = SceneState::Edit;

		m_ActiveScene = m_EditorScene;

		m_SceneHierachyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnScenePause()
	{
		if (m_SceneState == SceneState::Edit)
			return;

		m_ActiveScene->SetPaused(true);
	}

	void EditorLayer::OnDuplicateEntity()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		Entity selectedEntity = m_SceneHierachyPanel.GetSelectedEntity();
		if (selectedEntity)
			m_EditorScene->DuplicateEntity(selectedEntity);
	}
}
