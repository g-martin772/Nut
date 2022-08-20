 #pragma once

#include "Nut/Layer.h"
#include "../vendor/imgui/imgui.h"
#include "Nut/ImGui/imgui_impl_opengl3.h"
#include "../vendor/GLFW/include/GLFW/glfw3.h"


namespace Nut {
	class NUT_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();


		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
	static const char* ImGui_ImplGlfw_GetClipboardText(void* user_data);
	static void ImGui_ImplGlfw_SetClipboardText(void* user_data, const char* text);
}
