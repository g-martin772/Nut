#include "SceneHierachyPanel.h"
#include <imgui/imgui_internal.h>

#include "Nut/Scripting/ScriptEngine.h"

namespace Nut {

	extern const std::filesystem::path g_AssetPath;

	static const ImGuiTreeNodeFlags treeNodeFlags =
		ImGuiTreeNodeFlags_DefaultOpen |
		ImGuiTreeNodeFlags_AllowItemOverlap |
		ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_Framed |
		ImGuiTreeNodeFlags_FramePadding;

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T, typename UIFunc>
	static void DrawComponent(const std::string& name, Entity entity, UIFunc uiFunc) {
		if (!entity.HasComponent<T>())
			return;

		ImVec2 contenRegionAvail = ImGui::GetContentRegionAvail();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::PopStyleVar();
		ImGui::SameLine(contenRegionAvail.x - lineHeight * 0.5f);
		if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;

			ImGui::EndPopup();
		}

		if (open) {
			auto& component = entity.GetComponent<T>();
			uiFunc(component);
			ImGui::TreePop();
		}

		if (removeComponent)
			entity.RemoveComponent<T>();
	}

	SceneHierachyPanel::SceneHierachyPanel(const Ref<Scene>& scene) { SetContext(scene); }

	void SceneHierachyPanel::SetContext(const Ref<Scene>& scene) { m_Context = scene; m_SelectionContext = {}; }

	void SceneHierachyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierachy");

		m_Context->m_Registry.each([&](auto entityID) {
			Entity entity{ entityID, m_Context.get() };
		DrawEntityNode(entity);
			});

		if (ImGui::IsMouseClicked(0, false) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Components");

		if (m_SelectionContext) {
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();
	}





	void SceneHierachyPanel::DrawEntityNode(Entity entity)
	{
		auto& tc = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = (m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0 | ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_SpanAvailWidth;

		bool open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tc.c_str());
		if (ImGui::IsItemClicked()) {
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (open) {
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	void SceneHierachyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(TagComponent).hash_code(), treeNodeFlags, "Header")) {
				auto& tag = entity.GetComponent<TagComponent>().Tag;
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
				if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
					tag = std::string(buffer);
				}
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<TransformComponent>()) {
			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");

			if (open) {
				auto& tc = entity.GetComponent<TransformComponent>();
				DrawVec3Control("Translation", tc.Translation);
				DrawVec3Control("Scale", tc.Scalation, 1.0f);
				glm::vec3 rotation = glm::degrees(tc.Rotation);
				DrawVec3Control("Rotation", rotation);
				tc.Rotation = glm::radians(rotation);

				ImGui::TreePop();
			}
		}

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component) 
		{
			const char* projTypes[] = { "Perspective", "Orthographic" };
			const char* currentProjType = projTypes[(int)component.Camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjType)) {
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjType == projTypes[i];
					if (ImGui::Selectable(projTypes[i], isSelected)) {
						currentProjType = projTypes[i];
						component.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			if (ImGui::Checkbox("Primary", &component.Primary)) {

			}
			if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
				if (ImGui::Checkbox("FixedAspectRation", &component.FixedAspectRatio)) {

				}
				float orthoSize = component.Camera.GetOrthographicsSize();
				if (ImGui::DragFloat("OrthoSize", &orthoSize, 0.2f)) {
					component.Camera.SetOrthographicSize(orthoSize);
				}
				float nearClip = component.Camera.GetOrthoNearClip();
				if (ImGui::DragFloat("Near clip", &nearClip, 0.2f)) {
					component.Camera.SetOrthoNearClip(nearClip);
				}
				float farClip = component.Camera.GetOrthoFarClip();
				if (ImGui::DragFloat("Far clip", &farClip, 0.2f)) {
					component.Camera.SetOrthoFarClip(farClip);
				}
			}
			else if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
				float fov = glm::degrees(component.Camera.GetFOV());
				if (ImGui::DragFloat("FOV", &fov, 0.5f, 0.5f)) {
					if (fov > 0.5f)
						component.Camera.SetFOV(glm::radians(fov));
				}
				float nearClip = component.Camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near clip", &nearClip, 0.2f)) {
					component.Camera.SetPerspectiveNearClip(nearClip);
				}
				float farClip = component.Camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far clip", &farClip, 0.2f)) {
					component.Camera.SetPerspectiveFarClip(farClip);
				}
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component) 
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

			ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
					component.Texture = Texture2D::Create(texturePath.string());
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
		});

		DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

			ImGui::DragFloat("Radius", &component.Radius, 0.1f, 0.0f, 100.0f);
			ImGui::DragFloat("Fade", &component.Fade, 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat("Thickness", &component.Thickness, 0.1f, 0.0f, 1.0f);
		});

		DrawComponent<ScriptComponent>("Script Component (C#)", entity, [entity, scene = m_Context](auto& component) mutable
		{
			const auto& entityClasses = ScriptEngine::GetEntityClasses();
			bool scriptClassExists = entityClasses.find(component.Name) != entityClasses.end();
			static char buffer[64];
			strcpy_s(buffer, sizeof(buffer), component.Name.c_str());

			if (!scriptClassExists)
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f));

			if(ImGui::InputText("Class Name", buffer, sizeof(buffer)))
			{
				component.Name = buffer;
			}

			bool sceneRunning = scene->IsRunning();

			// WHAT_AM_I_EVEN_DOING_HELP_ME
			if (sceneRunning)
			{
				Ref<ScriptObject> scriptInstance = ScriptEngine::GetEntityScriptInstance(entity.GetComponent<IDComponent>().ID);
				if (scriptInstance)
				{
					const auto& fields = scriptInstance->GetScriptClass()->GetFields();
					for (const auto& [name, field] : fields)
					{
						if (field.Type == ScriptFieldType::Float)
						{
							float data = scriptInstance->GetFieldValue<float>(name);
							if (ImGui::DragFloat(name.c_str(), &data))
							{
								scriptInstance->SetFieldValue(name, data);
							}
						}
					}
				}
			}
			else 
			{
				if (scriptClassExists)
				{
					Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(component.Name);
					const auto& fields = entityClass->GetFields();

					auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);
					for (const auto& [name, field] : fields)
					{
						if (entityFields.find(name) != entityFields.end())
						{
							ScriptFieldInstance& scriptField = entityFields.at(name);

							if (field.Type == ScriptFieldType::Float)
							{
								float data = scriptField.GetValue<float>();
								if (ImGui::DragFloat(name.c_str(), &data))
									scriptField.SetValue(data);
							}
						}
						else
						{
							if (field.Type == ScriptFieldType::Float)
							{
								float data = 0.0f;
								if (ImGui::DragFloat(name.c_str(), &data))
								{
									ScriptFieldInstance& fieldInstance = entityFields[name];
									fieldInstance.Field = field;
									fieldInstance.SetValue(data);
								}
							}
						}
					}
				}
			}

			if (!scriptClassExists)
				ImGui::PopStyleColor();
		});

		DrawComponent<RigidBody2DComponent>("Rigidbody 2D", entity, [](auto& component) 
		{
			const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
			const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];
			if (ImGui::BeginCombo("Body Type", currentBodyTypeString)) {
				for (int i = 0; i < 2; i++) {
					bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
					if (ImGui::Selectable(bodyTypeStrings[i], isSelected)) {
						currentBodyTypeString = bodyTypeStrings[i];
						component.Type = (RigidBody2DComponent::BodyType)i;
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
		});

		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& component) 
		{
			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
			ImGui::DragFloat("Radius", &component.Radius);
			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
		});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component)
		{
			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
			ImGui::DragFloat2("Size", glm::value_ptr(component.Size));

			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
		});

		ImGui::NewLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (!m_SelectionContext.HasComponent<CameraComponent>()) {
				if (ImGui::MenuItem("Camera")) {
					m_SelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_SelectionContext.HasComponent<SpriteRendererComponent>()) {
				if (ImGui::MenuItem("Sprite Renderer")) {
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_SelectionContext.HasComponent<CircleRendererComponent>()) {
				if (ImGui::MenuItem("Cirlce Renderer")) {
					m_SelectionContext.AddComponent<CircleRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_SelectionContext.HasComponent<RigidBody2DComponent>()) {
				if (ImGui::MenuItem("Rigidbody 2D")) {
					m_SelectionContext.AddComponent<RigidBody2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_SelectionContext.HasComponent<BoxCollider2DComponent>()) {
				if (ImGui::MenuItem("Boxcollider 2D")) {
					m_SelectionContext.AddComponent<BoxCollider2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_SelectionContext.HasComponent<CircleCollider2DComponent>()) {
				if (ImGui::MenuItem("CircleCollider 2D")) {
					m_SelectionContext.AddComponent<CircleCollider2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (ImGui::MenuItem("C# Script")) {
				m_SelectionContext.AddComponent<ScriptComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();
	}

	void SceneHierachyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext = entity;
	}
}