#pragma once

#include "Nut/Core/Core.h"
#include "Nut/Core/Layer.h"
#include <vector>

namespace Nut {
	class NUT_API LayerStack {
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		int m_LayerInsert = 0;
	};
}
