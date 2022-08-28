#pragma once
#include <Nut/Camera/Camera.h>
#include <Nut/Camera/OrthographicCamera.h>
#include <Nut/Renderer/Texture.h>
#include <Nut/Renderer/EditorCamera.h>

namespace Nut {
	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f), float tilingFactor = 1.0f, int entityID = -1);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f), float tilingFactor = 1.0f, int entityID = -1);
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint = glm::vec4(1.0f), float tilingFactor = 1.0f, int entityID = -1);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint = glm::vec4(1.0f), float tilingFactor = 1.0f, int entityID = -1);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);

		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f), float tilingFactor = 1.0f, int entityID = -1);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f), float tilingFactor = 1.0f, int entityID = -1);
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint = glm::vec4(1.0f), float tilingFactor = 1.0f, int entityID = -1);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint = glm::vec4(1.0f), float tilingFactor = 1.0f, int entityID = -1);
		
		struct Statistics {
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static Statistics GetStats();
		static void ResetStats();
	};
 }