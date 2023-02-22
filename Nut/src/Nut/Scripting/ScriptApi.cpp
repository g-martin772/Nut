#include "ntpch.h"
#include "ScriptApi.h"

#include <glm/glm.hpp>

#include "mono/metadata/object.h"
#include "Nut/Core/UUID.h"
#include "ScriptEngine.h"
#include "Nut/Scene/Entity.h"
#include "Nut/Core/Input.h"

namespace Nut {

	#define NT_ADD_INTERNAL_CALL(name) mono_add_internal_call("Nut.NativeCalls::Native_" #name "", name);

	#pragma region Console
	static void Print(MonoString* text)
	{
		std::string textString = mono_string_to_utf8(text);
		NT_SCRIPT_TRACE("{0}", textString);
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


	void ScriptApi::RegisterInternalCalls()
	{
		NT_ADD_INTERNAL_CALL(Print);

		NT_ADD_INTERNAL_CALL(Entity_GetTranslation);
		NT_ADD_INTERNAL_CALL(Entity_SetTranslation);

		NT_ADD_INTERNAL_CALL(IsKeyDown);
	}
}
