 #pragma once

#include "Nut/Core/Layer.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Nut/ImGui/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"



namespace Nut {
	class NUT_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();


		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void SetConsumeEvents(bool consume) { m_ConsumeEvents = consume; }
	private:
		void SetDarkThemeColors();
	private:
		bool m_ConsumeEvents = true;
		float m_Time = 0.0f;
	};
	static const char* ImGui_ImplGlfw_GetClipboardText(void* user_data);
	static void ImGui_ImplGlfw_SetClipboardText(void* user_data, const char* text);
}
