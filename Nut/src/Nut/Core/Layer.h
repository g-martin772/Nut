#pragma once

#include "Nut/Core/Core.h"
#include "Nut/Events/Event.h"
#include "Nut/Core/Timestep.h"

namespace Nut {
	class NUT_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnDetach() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};

	
}
