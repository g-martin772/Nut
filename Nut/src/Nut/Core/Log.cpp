#include "ntpch.h"
#include "Log.h"

namespace Nut {
	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");

		Log::GetCoreLogger()->set_level(spdlog::level::trace);
		Log::GetClientLogger()->set_level(spdlog::level::trace);
		Log::GetScriptLogger()->set_level(spdlog::level::trace);
	}
}
