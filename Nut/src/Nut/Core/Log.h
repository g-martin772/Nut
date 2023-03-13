#pragma once

#include "Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

namespace Nut {
	class NUT_API Log {
	public:
		static void Init();

		static Ref<spdlog::logger> GetCoreLogger() {
			static Ref<spdlog::logger> s_CoreLogger = spdlog::stdout_color_mt("NUT");
			return s_CoreLogger;
		}

		static Ref<spdlog::logger> GetClientLogger() {
			static Ref<spdlog::logger> s_ClientLogger = spdlog::stdout_color_mt("APP");
			return s_ClientLogger;
		}

		static Ref<spdlog::logger> GetScriptLogger() {
			static Ref<spdlog::logger> s_ScriptLogger = spdlog::stdout_color_mt("SCRIPT");
			return s_ScriptLogger;
		}
	};
}

#define NT_CORE_FATAL(...)		::Nut::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define NT_CORE_ERROR(...)		::Nut::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NT_CORE_WARN(...)		::Nut::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NT_CORE_INFO(...)		::Nut::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NT_CORE_TRACE(...)		::Nut::Log::GetCoreLogger()->trace(__VA_ARGS__)

#define NT_SCRIPT_FATAL(...)	::Nut::Log::GetScriptLogger()->critical(__VA_ARGS__)
#define NT_SCRIPT_ERROR(...)	::Nut::Log::GetScriptLogger()->error(__VA_ARGS__)
#define NT_SCRIPT_WARN(...)		::Nut::Log::GetScriptLogger()->warn(__VA_ARGS__)
#define NT_SCRIPT_INFO(...)		::Nut::Log::GetScriptLogger()->info(__VA_ARGS__)
#define NT_SCRIPT_TRACE(...)	::Nut::Log::GetScriptLogger()->trace(__VA_ARGS__)

#define NT_FATAL(...)			::Nut::Log::GetClientLogger()->critical(__VA_ARGS__)
#define NT_ERROR(...)			::Nut::Log::GetClientLogger()->error(__VA_ARGS__)
#define NT_WARN(...)			::Nut::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NT_INFO(...)			::Nut::Log::GetClientLogger()->info(__VA_ARGS__)
#define NT_TRACE(...)			::Nut::Log::GetClientLogger()->trace(__VA_ARGS__)