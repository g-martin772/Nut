#pragma once

#include "Nut/Core/Application.h"
#include "Nut/Core/Log.h"

#ifdef NT_PLATFORM_WINDOWS

extern Nut::Application* Nut::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv) {
	Nut::Log::Init();

	NT_PROFILE_BEGIN_SESSION("Startup", "NutProfile-Startup.json");
	auto app = Nut::CreateApplication({ argc, argv });
	NT_PROFILE_END_SESSION();

	NT_PROFILE_BEGIN_SESSION("Runtime", "NutProfile-Runtime.json");
	app->Run();
	NT_PROFILE_END_SESSION();

	NT_PROFILE_BEGIN_SESSION("Shutdown", "NutProfile-Shutdown.json");
	delete app;
	NT_PROFILE_END_SESSION();
}

#else
#error Nuts for Windows
#endif