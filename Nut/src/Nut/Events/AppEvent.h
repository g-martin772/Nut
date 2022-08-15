#pragma once

#include "Event.h"

namespace Nut{
	class NUT_API WindowResizeEvent : public Event {
	public:
		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

			WindowResizeEvent(unsigned int x, unsigned int y)
			: m_Width(x), m_Height(y) { }

	protected:
		unsigned int m_Width, m_Height;
	};

	class NUT_API WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class NUT_API AppTickEvent : public Event {
	public:
		AppTickEvent() {}
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class NUT_API AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class NUT_API AppRenderEvent : public Event {
	public:
		AppRenderEvent() {}
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class NUT_API ButtonEvent : public Event {
	public:
		inline int GetButton() const { return m_button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput)

	protected:
		ButtonEvent(int button)
			: m_button(button) { }

		int m_button;
	};

	class NUT_API ButtonPressedEvent : public ButtonEvent {
	public:
		ButtonPressedEvent(int button)
			: ButtonEvent(button) { }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(ButtonPressed)
	};

	class NUT_API ButtonReleasedEvent : public ButtonEvent {
	public:
		ButtonReleasedEvent(int button)
			: ButtonEvent(button) { }


		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(ButtonReleased)
	};
}