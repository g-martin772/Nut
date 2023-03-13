#pragma once
#include "Nut/Core/UUID.h"

namespace Nut {
	class ScriptApi {
	public:
		static void RegisterInternalCalls();
		static void RegisterComponents();
	};
}