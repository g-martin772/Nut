#pragma once

#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Nut {
	class NUT_API Log {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define NT_CORE_FATAL(...)	 ::Nut::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define NT_CORE_ERROR(...)	 ::Nut::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NT_CORE_WARN(...)	 ::Nut::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NT_CORE_INFO(...)	 ::Nut::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NT_CORE_TRACE(...)	 ::Nut::Log::GetCoreLogger()->trace(__VA_ARGS__)
		
#define NT_FATAL(...)		 ::Nut::Log::GetClientLogger()->critical(__VA_ARGS__)
#define NT_ERROR(...)		 ::Nut::Log::GetClientLogger()->error(__VA_ARGS__)
#define NT_WARN(...)		 ::Nut::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NT_INFO(...)		 ::Nut::Log::GetClientLogger()->info(__VA_ARGS__)
#define NT_TRACE(...)		 ::Nut::Log::GetClientLogger()->trace(__VA_ARGS__)