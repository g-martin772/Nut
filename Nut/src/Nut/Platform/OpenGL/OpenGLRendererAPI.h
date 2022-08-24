#pragma once

#include "Nut/Renderer/RendererAPI.h"

namespace Nut {
	class OpenGLRendererAPI : public RendererAPI {
	public:
		void Init() override;
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
}
