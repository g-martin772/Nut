#pragma once

#include "Nut/Layer.h"
#include "../vendor/imgui/imgui.h"
#include "Nut/Platform/OpenGL/ImGuiOpenGL3.h"
#include "../vendor/GLFW/include/GLFW/glfw3.h"
#include "Nut/Application.h"

namespace Nut {
	class NUT_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();


		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;
	private:
		float m_Time = 0.0f;
	};
}
