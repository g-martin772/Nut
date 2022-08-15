#include "Application.h"

#include "Events/AppEvent.h"
#include "Log.h"

namespace Nut {
	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::Run() {
		WindowResizeEvent e(1280, 720);
		NT_TRACE(e);

		while (true);
	}
}
