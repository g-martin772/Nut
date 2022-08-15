#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Nut {
	class NUT_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};


	//To be defined by client!
	Application* CreateApplication();
}

