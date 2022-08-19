//Inspired by Hazel Engine from TheCherno

#pragma once

#include "ntpch.h"

#ifdef NT_PLATFORM_WINDOWS
	#ifdef NT_BUILD_DLL
		#define NUT_API __declspec(dllexport)
	#else
		#define NUT_API __declspec(dllimport)
	#endif
#else
	#error Nuts for Windows
#endif

#ifdef NT_DEBUG
	#define NT_CORE_ASSERTS
#endif

#ifdef NT_RELEASE

#endif

#ifdef NT_DIST

#endif

#ifdef NT_CORE_ASSERTS
	#define NT_ASSERT(x, ...) { if(!(x)) {NT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define NT_CORE_ASSERT(x, ...) { if(!(x)) {NT_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define NT_ASSERT(x, ...)
	#define NT_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

