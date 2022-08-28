#pragma once

#include "Nut/Scene/Scene.h"
#include "../vendor/UUIDv4/endianness.h"
#include "../vendor/UUIDv4/uuid_v4.h"

namespace Nut {
	class Scene;
	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene) : m_Scene(scene), m_EntityHandle(handle) {}
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent() { return m_Scene->m_Registry.get<T>(m_EntityHandle); }

		template<typename T>
		bool HasComponent() { return m_Scene->m_Registry.any_of<T>(m_EntityHandle); }

		template<typename T>
		void RemoveComponent() { m_Scene->m_Registry.remove<T>(m_EntityHandle); }

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator entt::entity() const { return m_EntityHandle; }

		bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return !operator==(other); }
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene;
	};
}