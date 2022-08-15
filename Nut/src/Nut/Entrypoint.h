#pragma once

#ifdef NT_PLATFORM_WINDOWS

extern Nut::Application* Nut::CreateApplication();

int main(int argc, char** argv) {
	Nut::Log::Init();
	NT_CORE_ERROR("Initialized Log");
	NT_WARN("Logger running");

	auto app = Nut::CreateApplication();
	app->Run();
	delete app;
}

#else
	#error Nuts for Windows
#endif