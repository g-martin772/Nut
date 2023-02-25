#include "ntpch.h"
#include "ScriptApi.h"

#include <glm/glm.hpp>

#include "mono/metadata/object.h"
#include "Nut/Core/UUID.h"
#include "ScriptEngine.h"
#include "Nut/Scene/Entity.h"
#include "Nut/Core/Input.h"
#include "mono/metadata/reflection.h"

namespace Nut {

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;
	#define NT_ADD_INTERNAL_CALL(name) mono_add_internal_call("Nut.InternalCalls::Native_" #name "", name);

	#pragma region Console
	static void Print(MonoString* text)
	{
		std::string textString = mono_string_to_utf8(text);
		NT_SCRIPT_TRACE("{0}", textString);
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
	static void Entity_GetTranslation(UUID entityID, glm::vec3* translation)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		*translation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void Entity_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Entity entity = ScriptEngine::GetCurrentScene()->GetEntityByUUID(entityID);
		auto& tc = entity.GetComponent<TransformComponent>();
		tc.Translation = *translation;
	}
	#pragma endregion


	#pragma region Input
	static bool IsKeyDown(int keycode)
	{
		return Input::IsKeyPressed(keycode);
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

		NT_ADD_INTERNAL_CALL(Entity_GetTranslation);
		NT_ADD_INTERNAL_CALL(Entity_SetTranslation);

		NT_ADD_INTERNAL_CALL(IsKeyDown);
	}
}
