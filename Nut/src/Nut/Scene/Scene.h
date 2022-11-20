#pragma once

#include <enTT/entt.hpp>

#include "Nut/Core/Timestep.h"
#include "Nut/Renderer/EditorCamera.h"


class b2World;

namespace Nut {
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		Entity GetPrimaryCameraEntity();

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		b2World* m_World = nullptr;

		friend class Entity;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
	};
}