#pragma once

#include "ntpch.h"

#include "Nut/Core.h"
#include "Nut/Events/Event.h"

namespace Nut {
	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(unsigned int width = 1280,
					unsigned int height = 720,
					const std::string& title = "Nut Engine")
			: Title(title), Width(width), Height(height) {}
	};

	class NUT_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window(){}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}
