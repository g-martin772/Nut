#pragma once

#include "ntpch.h"
#include "Application.h"
#include "Log.h"

#ifdef NT_PLATFORM_WINDOWS

extern Nut::Application* Nut::CreateApplication();

int main(int argc, char** argv) {
	Nut::Log::Init();

	auto app = Nut::CreateApplication();
	app->Run();
	delete app;
}

#else
	#error Nuts for Windows
#endif