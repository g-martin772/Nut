#pragma once

#include <enTT/entt.hpp>

#include "Nut/Core/Timestep.h"
#include "Nut/Renderer/EditorCamera.h"
#include "../Core/UUID.h"


class b2World;

namespace Nut {
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);
		Entity GetEntityByUUID(UUID uuid);
		Entity FindEntityByName(std::string_view name);

		Entity GetPrimaryCameraEntity();

		bool IsRunning() const { return m_Running; }

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnSimulationStart();
		void OnSimulationStop();

		static Ref<Scene> Copy(Ref<Scene> other);
		void DuplicateEntity(Entity entity);

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);
		void OnUpdateSimulation(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		void OnPhysics2DStart();
		void OnPhysics2DStop();

		void OnScriptingStart();
		void OnScriptingStop();

		void RenderScene(EditorCamera& camera);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;
		bool m_Running = false;

		std::unordered_map<UUID, entt::entity> m_EntityMap;

		b2World* m_PhysicsWorld = nullptr;

		friend class Entity;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
	};
}