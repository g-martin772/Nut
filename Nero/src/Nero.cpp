#include <Nut.h>
#include <Nut/Core/Entrypoint.h>

#include "EditorLayer.h"

namespace Nut {
	class NeroEditor : public Nut::Application {
	public:
		NeroEditor(ApplicationCommandLineArgs args) : Application("Nero Editor", 1500, 900, args) {
			PushLayer(new EditorLayer());
		}

		~NeroEditor() {

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args) {
		return new NeroEditor(args);
	}
}
 