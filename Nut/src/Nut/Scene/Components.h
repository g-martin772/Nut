#pragma once

#include <enTT/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <functional>

#include "Nut/Scene/SceneCamera.h"
#include "Nut/Scene/ScriptableEntity.h"

namespace Nut {
	struct TagComponent {
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(std::string& tag) : Tag(tag) {}

		operator std::string() { return Tag; }
		operator const std::string() const { return Tag; }
	};

	struct MeshComponent {

	};

	struct TransformComponent {
		glm::mat4 Transform{ 1.0f };
		glm::vec3 Translation{ 0.0f };
		glm::vec3 Scalation{ 1.0f };
		glm::vec3 Rotation{ 0.0f };
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) : Transform(transform) {}

		glm::mat4 GetTransform() {
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scalation);;
		}
	};

	struct SpriteRendererComponent {
		glm::vec4 Color{ 1.0f };
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}

		operator glm::vec4& () { return Color; }
		operator const glm::vec4& () const { return Color; }
	};

	struct CameraComponent {
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateFunction)();
		void (*DestroyInstanceFunction)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateFunction = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyInstanceFunction = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}