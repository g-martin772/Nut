#pragma once

#include "Nut/Renderer/RenderCommand.h"
#include <Nut/Renderer/OrthographicCamera.h>
#include <Nut/Renderer/Shader.h>

namespace Nut {
	class Renderer {
	public:
		Renderer();
		~Renderer();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetApi(); }
	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* m_SceneData;
	};

}
