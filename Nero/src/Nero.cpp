#include <Nut.h>
#include <Nut/Core/Entrypoint.h>

#include "EditorLayer.h"

namespace Nut {
	class NeroEditor : public Nut::Application {
	public:
		NeroEditor() : Application("Nero Editor", 1500, 900) {
			PushLayer(new EditorLayer());
		}

		~NeroEditor() {

		}
	};

	Application* CreateApplication() {
		return new NeroEditor();
	}
}
 