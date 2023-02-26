#include "ntpch.h"
#include "ScriptApi.h"

#include <glm/glm.hpp>

#include "mono/metadata/object.h"
#include "Nut/Core/UUID.h"
#include "ScriptEngine.h"
#include "Nut/Scene/Entity.h"
#include "Nut/Core/Input.h"
#include "mono/metadata/reflection.h"
#include "box2d/b2_body.h"

namespace Nut {

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;
	#define NT_ADD_INTERNAL_CALL(name) mono_add_internal_call("Nut.InternalCalls::Native_" #name "", name);

	#pragma region Console
	static void Print(MonoString* text, int level)
	{
		std::string textString = mono_string_to_utf8(text);
		switch (level)
		{
			case 1: NT_SCRIPT_TRACE("{0}", textString);	break;
			case 2: NT_SCRIPT_INFO("{0}", textString);	break;
			case 3:	NT_SCRIPT_WARN("{0}", textString);	break;
			case 4: NT_SCRIPT_ERROR("{0}", textString);	break;
			case 5: NT_SCRIPT_FATAL("{0}", textString);	break;
		}
	}
	#pragma endregion


	#pragma region Entity
	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		NT_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end(), "Component type not found");
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}
	#pragma endregion


	#pragma region Basic Components
	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* translation)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		*translation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		auto& tc = entity.GetComponent<TransformComponent>();
		tc.Translation = *translation;
	}

	static void TransformComponent_GetRotation(UUID entityID, glm::vec3* rotation)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		*rotation = entity.GetComponent<TransformComponent>().Rotation;
	}

	static void TransformComponent_SetRotation(UUID entityID, glm::vec3* rotation)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		auto& tc = entity.GetComponent<TransformComponent>();
		tc.Rotation = *rotation;
	}

	static void TransformComponent_GetScale(UUID entityID, glm::vec3* scale)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		*scale = entity.GetComponent<TransformComponent>().Scalation;
	}

	static void TransformComponent_SetScale(UUID entityID, glm::vec3* scale)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		auto& tc = entity.GetComponent<TransformComponent>();
		tc.Scalation = *scale;
	}



	static void SpriteRenderComponent_GetColor(UUID entityID, glm::vec4* color)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		*color = entity.GetComponent<SpriteRendererComponent>().Color;
	}

	static void SpriteRenderComponent_SetColor(UUID entityID, glm::vec4* color)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		auto& src = entity.GetComponent<SpriteRendererComponent>();
		src.Color = *color;
	}
	#pragma endregion


	#pragma region Input
	static bool IsKeyDown(int keycode)
	{
		return Input::IsKeyPressed(keycode);
	}
	#pragma endregion


	#pragma region Physics2D
	static void RigidBody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* pos, bool wake)
	{
		RigidBody2DComponent rb2d = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID).GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(pos->x, pos->y), wake);
	}

	static void RigidBody2DComponent_ApplyLinearImpulseCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		RigidBody2DComponent rb2d = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID).GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}
	#pragma endregion

	template<typename... Component>
	static void RegisterComponent()
	{
		([]() {
			std::string_view typeName = typeid(Component).name();
			size_t pos = typeName.find_last_of(':');
			std::string_view structName = typeName.substr(pos + 1);
			std::string managedTypename = fmt::format("Nut.Scene.{}", structName);

			MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
			if (!managedType)
			{
				NT_CORE_ERROR("Could not find component type {}", managedTypename);
				return;
			}
			s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
		}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptApi::RegisterComponents()
	{
		RegisterComponent(AllComponents{});
	}


	void ScriptApi::RegisterInternalCalls()
	{
		NT_ADD_INTERNAL_CALL(Print);

		NT_ADD_INTERNAL_CALL(Entity_HasComponent);

		NT_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		NT_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		NT_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		NT_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		NT_ADD_INTERNAL_CALL(TransformComponent_GetScale);
		NT_ADD_INTERNAL_CALL(TransformComponent_SetScale);

		NT_ADD_INTERNAL_CALL(SpriteRenderComponent_GetColor);
		NT_ADD_INTERNAL_CALL(SpriteRenderComponent_SetColor);

		NT_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulse);
		NT_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulseCenter);

		NT_ADD_INTERNAL_CALL(IsKeyDown);
	}
}