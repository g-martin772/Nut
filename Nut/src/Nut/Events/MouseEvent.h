#pragma once

#include "Event.h"

namespace Nut {

	class NUT_API MouseMovedEvent : public Event {
	public:
		inline float GetX() const { return m_x; }
		inline float GetY() const { return m_y; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_x << ", " << m_y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

		MouseMovedEvent(float x, float y)
			: m_x(x), m_y(y) { }

	protected:
		float m_x, m_y;
	};

	class NUT_API MouseScrolledEvent : public Event {
	public:
		inline float GetXOffset() const { return m_xO; }
		inline float GetYOffset() const { return m_yO; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

		MouseScrolledEvent(float xO, float yO)
			: m_xO(xO), m_yO(yO) { }

	protected:
		float m_xO, m_yO;
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

