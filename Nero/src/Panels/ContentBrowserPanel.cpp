#include "ContentBrowserPanel.h"

#include "Nut.h"

#include "imgui.h"

namespace Nut {
	ContentBrowserPanel::ContentBrowserPanel() 
		: m_BaseDir(Project::GetAssetDirectory()), m_CurrentDir(m_BaseDir)
	{
		m_DirectoryIcon = Texture2D::Create("resources/icons/fileicons/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("resources/icons/fileicons/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDir != std::filesystem::path(m_BaseDir)) {
			if (ImGui::Button("<-")) {
				m_CurrentDir = m_CurrentDir.parent_path();
			}
		}


		static float padding = 12.0f;
		static float thumbnailSize = 82.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);


		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDir))
		{
			const auto& path = directoryEntry.path();
			std::filesystem::path relativePath(path);
			std::string filenameString = relativePath.filename().string();

			ImGui::PushID(filenameString.c_str());
			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource()) {
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDir /= path.filename();

			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}


		ImGui::Columns(1);

		ImGui::End();
	}
}

