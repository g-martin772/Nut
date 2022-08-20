#pragma once

#include "Nut/Layer.h"
#include "../vendor/imgui/imgui.h"
#include "Nut/Platform/OpenGL/ImGuiOpenGL3.h"
#include "../vendor/GLFW/include/GLFW/glfw3.h"
#include "Nut/Application.h"
#include "Nut/Events/KeyEvents.h"
#include "Nut/Events/MouseEvent.h"


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
		bool OnMouseButtonPressedEvent(ButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(ButtonReleasedEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);


		float m_Time = 0.0f;
	};
	static const char* ImGui_ImplGlfw_GetClipboardText(void* user_data);
	static void ImGui_ImplGlfw_SetClipboardText(void* user_data, const char* text);
}
