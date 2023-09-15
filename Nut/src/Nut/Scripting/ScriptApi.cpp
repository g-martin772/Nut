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
	#define NT_ADD_INTERNAL_CALL(name) mono_add_internal_call("Nut.InternalCalls::Native_" #name "", name)

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
	static bool Entity_HasComponent(const UUID entityId, MonoReflectionType* componentType)
	{
		const Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityId);
		
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		NT_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end(), "Component type not found");
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static MonoObject* GetScriptInstance(const UUID entityId)
	{
		return ScriptEngine::GetManagedInstance(entityId);
	}

	static uint64_t Entity_FindEntityByName(MonoString* name)
	{
		char* nameCStr = mono_string_to_utf8(name);

		Scene* scene = ScriptEngine::GetCurrentScene();
		NT_CORE_ASSERT(scene, "No active scrip scene");
		Entity entity = scene->FindEntityByName(nameCStr);
		mono_free(nameCStr);

		if (!entity)
			return 0;

		return entity.GetComponent<IDComponent>().ID;
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


	static void CircleRenderComponent_GetColor(UUID entityID, glm::vec4* color)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		*color = entity.GetComponent<CircleRendererComponent>().Color;
	}

	static void CircleRenderComponent_SetColor(UUID entityID, glm::vec4* color)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		auto& crc = entity.GetComponent<CircleRendererComponent>();
		crc.Color = *color;
	}

	static float CircleRenderComponent_GetFade(UUID entityID)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		return entity.GetComponent<CircleRendererComponent>().Fade;
	}

	static void CircleRenderComponent_SetFade(UUID entityID, float fade)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		auto& bc = entity.GetComponent<CircleRendererComponent>();
		bc.Fade = fade;
	}

	static float CircleRenderComponent_GetRadius(UUID entityID)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		return entity.GetComponent<CircleRendererComponent>().Radius;
	}

	static void CircleRenderComponent_SetRadius(UUID entityID, float radius)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		auto& bc = entity.GetComponent<CircleRendererComponent>();
		bc.Radius = radius;
	}

	static float CircleRenderComponent_GetThickness(UUID entityID)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		return entity.GetComponent<CircleRendererComponent>().Thickness;
	}

	static void CircleRenderComponent_SetThickness(UUID entityID, float thickness)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		auto& bc = entity.GetComponent<CircleRendererComponent>();
		bc.Thickness = thickness;
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

	static void RigidBody2DComponent_ApplyForce(UUID entityID, glm::vec2* impulse, glm::vec2* pos, bool wake)
	{
		RigidBody2DComponent rb2d = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID).GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyForce(b2Vec2(impulse->x, impulse->y), b2Vec2(pos->x, pos->y), wake);
	}

	static void RigidBody2DComponent_ApplyForceCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		RigidBody2DComponent rb2d = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID).GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyForceToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	static void RigidBody2DComponent_ApplyTorque(UUID entityID, float torque, bool wake)
	{
		RigidBody2DComponent rb2d = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID).GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyTorque(torque, wake);
	}

	static void BoxCollider2DComponent_GetSize(UUID entityID, glm::vec2* size)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		*size = entity.GetComponent<BoxCollider2DComponent>().Size;
	}

	static void BoxCollider2DComponent_SetSize(UUID entityID, glm::vec2* size)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		auto& bc = entity.GetComponent<BoxCollider2DComponent>();
		bc.Size = *size;
	}

	static void BoxCollider2DComponent_GetOffset(UUID entityID, glm::vec2* offset)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		*offset = entity.GetComponent<BoxCollider2DComponent>().Offset;
	}

	static void BoxCollider2DComponent_SetOffset(UUID entityID, glm::vec2* offset)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		auto& bc = entity.GetComponent<BoxCollider2DComponent>();
		bc.Offset = *offset;
	}

	static float BoxCollider2DComponent_GetDensity(UUID entityID)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		return entity.GetComponent<BoxCollider2DComponent>().Density;
	}

	static void BoxCollider2DComponent_SetDensity(UUID entityID, float density)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		auto& bc = entity.GetComponent<BoxCollider2DComponent>();
		bc.Density = density;
	}

	static float BoxCollider2DComponent_GetFriction(UUID entityID)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		return entity.GetComponent<BoxCollider2DComponent>().Friction;
	}

	static void BoxCollider2DComponent_SetFriction(UUID entityID, float friction)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		auto& bc = entity.GetComponent<BoxCollider2DComponent>();
		bc.Friction = friction;
	}

	static float BoxCollider2DComponent_GetRestitution(UUID entityID)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		return entity.GetComponent<BoxCollider2DComponent>().Restitution;
	}

	static void BoxCollider2DComponent_SetRestitution(UUID entityID, float restitution)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		auto& bc = entity.GetComponent<BoxCollider2DComponent>();
		bc.Restitution = restitution;
	}

	static float BoxCollider2DComponent_GetRestitutionThreshold(UUID entityID)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		return entity.GetComponent<BoxCollider2DComponent>().RestitutionThreshold;
	}

	static void BoxCollider2DComponent_SetRestitutionThreshold(UUID entityID, float restitutionThreshold)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		auto& bc = entity.GetComponent<BoxCollider2DComponent>();
		bc.RestitutionThreshold = restitutionThreshold;
	}
	#pragma endregion

	template<typename... Component>
	static void RegisterComponent()
	{
		([]() {
			const std::string_view typeName = typeid(Component).name();
			const size_t pos = typeName.find_last_of(':');
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
		s_EntityHasComponentFuncs.clear();
		RegisterComponent(AllComponents{});
	}


	void ScriptApi::RegisterInternalCalls()
	{
		NT_ADD_INTERNAL_CALL(Print);

		NT_ADD_INTERNAL_CALL(GetScriptInstance);
		NT_ADD_INTERNAL_CALL(Entity_HasComponent);
		NT_ADD_INTERNAL_CALL(Entity_FindEntityByName);

		NT_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		NT_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		NT_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		NT_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		NT_ADD_INTERNAL_CALL(TransformComponent_GetScale);
		NT_ADD_INTERNAL_CALL(TransformComponent_SetScale);

		NT_ADD_INTERNAL_CALL(SpriteRenderComponent_GetColor);
		NT_ADD_INTERNAL_CALL(SpriteRenderComponent_SetColor);

		NT_ADD_INTERNAL_CALL(CircleRenderComponent_GetColor);
		NT_ADD_INTERNAL_CALL(CircleRenderComponent_SetColor);
		NT_ADD_INTERNAL_CALL(CircleRenderComponent_GetRadius);
		NT_ADD_INTERNAL_CALL(CircleRenderComponent_SetRadius);
		NT_ADD_INTERNAL_CALL(CircleRenderComponent_GetFade);
		NT_ADD_INTERNAL_CALL(CircleRenderComponent_SetFade);
		NT_ADD_INTERNAL_CALL(CircleRenderComponent_GetThickness);
		NT_ADD_INTERNAL_CALL(CircleRenderComponent_SetThickness);

		NT_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulse);
		NT_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulseCenter);
		NT_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyForce);
		NT_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyForceCenter);
		NT_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyTorque);

		NT_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetOffset);
		NT_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetOffset);
		NT_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetSize);
		NT_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetSize);
		NT_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetDensity);
		NT_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetDensity);
		NT_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetRestitution);
		NT_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetRestitution);
		NT_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetRestitutionThreshold);
		NT_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetRestitutionThreshold);

		NT_ADD_INTERNAL_CALL(IsKeyDown);
	}
}
