#pragma once

#include "Nut/Renderer/RenderCommand.h"

namespace Nut {
	class Renderer {
	public:
		Renderer();
		~Renderer();

		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetApi(); }
	};

}
