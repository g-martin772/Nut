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

#define BIT(x) (1 << x)
