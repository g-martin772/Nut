#include "ntpch.h"
#include "ScriptApi.h"

#include "mono/metadata/object.h"

namespace Nut {
	#define NT_ADD_INTERNAL_CALL(name) mono_add_internal_call("Nut.NativeCalls::Native_" #name "", name);

	static void Print(MonoString* text)
	{
		std::string textString = mono_string_to_utf8(text);
		NT_SCRIPT_TRACE("{0}", textString);
	}

	void ScriptApi::RegisterInternalCalls()
	{
		NT_ADD_INTERNAL_CALL(Print);
	}
}
