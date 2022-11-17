#include "SceneHierachyPanel.h"
#include <imgui/imgui_internal.h>

namespace Nut {
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
				strcpy_s(buffer, 256, tag.c_str());
				if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
					tag = std::string(buffer);
				}
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<TransformComponent>()) {
			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");

			if (open){
				auto& tc = entity.GetComponent<TransformComponent>();
				DrawVec3Control("Translation", tc.Translation);
				DrawVec3Control("Scale", tc.Scalation, 1.0f);
				glm::vec3 rotation = glm::degrees(tc.Rotation);
				DrawVec3Control("Rotation", rotation);
				tc.Rotation = glm::radians(rotation);

				ImGui::TreePop();
			}
		}

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
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

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
		});



		ImGui::NewLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				m_SelectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Renderer"))
			{
				m_SelectionContext.AddComponent<SpriteRendererComponent>();
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