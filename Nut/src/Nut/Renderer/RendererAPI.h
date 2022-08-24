#pragma once

#include "Nut/Renderer/Buffer.h"
#include "Nut/Renderer/VertexArray.h"
#include "../vendor/GLM/glm/glm.hpp"
#include <memory>

namespace Nut{
	class RendererAPI {
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1,
			Vulcan = 2,
			Medal = 3,
			DirectX9 = 9,
			DirextX10 = 10,
			DirectX11 = 11,
			DirectX12 = 12
		};

		virtual void Init() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetApi() { return s_API; }
	private:
		static API s_API;
	};
}
