#pragma once

#include "Nut.h"

namespace Nut {
	class SceneHierachyPanel {
	public:
		SceneHierachyPanel() = default;
		SceneHierachyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>&);

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}