#pragma once

#include "ntpch.h"
#include "Nut/Core.h"
#include "Nut/Events/Event.h"

namespace Nut {
	class NUT_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnUpdate() {}
		virtual void OnDetach() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};

	
}
