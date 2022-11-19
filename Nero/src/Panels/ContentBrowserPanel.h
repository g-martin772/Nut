#pragma once

#include "Nut/Renderer/Texture.h"

#include <filesystem>

namespace Nut{
	class ContentBrowserPanel {
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDir;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};
}