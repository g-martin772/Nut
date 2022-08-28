#include "ntpch.h"
#include "Renderer2D.h"
#include <Nut/Renderer/VertexArray.h>
#include <Nut/Renderer/Shader.h>
#include <Nut/Renderer/RenderCommand.h>

#include <../vendor/glm/glm/gtc/matrix_transform.hpp>

namespace Nut {
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
		//editor-only
		int EntityID = -1;
	};

	struct Renderer2DStorage {
		static const uint32_t MaxQuad = 200000;
		static const uint32_t MaxVertices = MaxQuad * 4;
		static const uint32_t MaxIndices = MaxQuad * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexBuffer> VertexBuffer;
		Ref<VertexArray> VertexArray;
		Ref<Shader> TextureShader2D;
		Ref<Texture2D> WhitePixelx1Texture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		NT_PROFILE_FUNCTION();
		s_Data = new Renderer2DStorage;

		s_Data->TextureShader2D = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->VertexArray = VertexArray::Create();

		s_Data->VertexBuffer = VertexBuffer::Create(s_Data->MaxVertices * sizeof(QuadVertex));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Pos" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" },
			{ ShaderDataType::Int, "a_EntityID" }
		};
		s_Data->VertexBuffer->SetLayout(layout);
		s_Data->VertexArray->AddVertexBuffer(s_Data->VertexBuffer);

		s_Data->QuadVertexBufferBase = new QuadVertex[s_Data->MaxVertices];

		uint32_t* indices = new uint32_t[s_Data->MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i <  s_Data->MaxIndices; i += 6) {
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, s_Data->MaxIndices);
		s_Data->VertexArray->AddIndexBuffer(indexBuffer);
		delete[] indices;

		s_Data->WhitePixelx1Texture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhitePixelx1Texture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		int32_t samplers[s_Data->MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data->MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data->TextureShader2D->Bind();
		s_Data->TextureShader2D->SetIntArr("u_Textures", samplers, s_Data->MaxTextureSlots);

		s_Data->TextureSlots[0] = s_Data->WhitePixelx1Texture;

		s_Data->QuadVertexPositions[0] = { -0.5, -0.5, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[1] = {  0.5, -0.5, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[2] = {  0.5,  0.5, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[3] = { -0.5,  0.5, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);
		s_Data->TextureShader2D->Bind();
		s_Data->TextureShader2D->SetMat4("u_VPM", viewProj);

		s_Data->TextureSlotIndex = 1;
		s_Data->QuadIndexCount = 0;
		s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		NT_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetViewProjection();

		s_Data->TextureShader2D->Bind();
		s_Data->TextureShader2D->SetMat4("u_VPM", viewProj);

		s_Data->TextureSlotIndex = 1;
		s_Data->QuadIndexCount = 0;
		s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->TextureShader2D->Bind();
		s_Data->TextureShader2D->SetMat4("u_VPM", camera.GetViewProjectionMatrix());

		s_Data->TextureSlotIndex = 1;
		s_Data->QuadIndexCount = 0;
		s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		NT_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data->QuadVertexBufferPtr - (uint8_t*)s_Data->QuadVertexBufferBase;
		s_Data->VertexBuffer->SetData(s_Data->QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		NT_PROFILE_FUNCTION();
		s_Data->Stats.DrawCalls++;
		for (uint32_t i = 0; i < s_Data->TextureSlotIndex; i++) {
			s_Data->TextureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_Data->VertexArray, s_Data->QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		NT_PROFILE_FUNCTION();
		DrawQuad({ pos.x, pos.y, 0.0f }, size, color, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		NT_PROFILE_FUNCTION();

		if (s_Data->QuadIndexCount >= s_Data->MaxIndices) {
			EndScene();
			s_Data->TextureSlotIndex = 1;
			s_Data->QuadIndexCount = 0;
			s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
		}

		s_Data->Stats.QuadCount++;

		s_Data->QuadVertexBufferPtr->Position = pos;
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = 0;
		s_Data->QuadVertexBufferPtr->TilingFactor = 1;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = {pos.x + size.x, pos.y, 0.0f};
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = 0;
		s_Data->QuadVertexBufferPtr->TilingFactor = 1;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = { pos.x + size.x, pos.y + size.y, 0.0f };
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = 0;
		s_Data->QuadVertexBufferPtr->TilingFactor = 1;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = { pos.x, pos.y + size.y, 0.0f };
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = 0;
		s_Data->QuadVertexBufferPtr->TilingFactor = 1;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor, int entityID)
	{
		NT_PROFILE_FUNCTION();
		DrawQuad({ pos.x, pos.y, 0.0f }, size, texture, tint, tilingFactor, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor, int entityID)
	{
		NT_PROFILE_FUNCTION();

		if (s_Data->QuadIndexCount >= s_Data->MaxIndices) {
			EndScene();
			s_Data->TextureSlotIndex = 1;
			s_Data->QuadIndexCount = 0;
			s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
		}

		s_Data->Stats.QuadCount++;

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data->TextureSlotIndex; i++) {
			if (*s_Data->TextureSlots[i] == *texture) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data->TextureSlotIndex;
			s_Data->TextureSlots[s_Data->TextureSlotIndex] = texture;
			s_Data->TextureSlotIndex++;
		}

		s_Data->QuadVertexBufferPtr->Position = pos;
		s_Data->QuadVertexBufferPtr->Color = tint;
		s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = { pos.x + size.x, pos.y, 0.0f };
		s_Data->QuadVertexBufferPtr->Color = tint;
		s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = { pos.x + size.x, pos.y + size.y, 0.0f };
		s_Data->QuadVertexBufferPtr->Color = tint;
		s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = { pos.x, pos.y + size.y, 0.0f };
		s_Data->QuadVertexBufferPtr->Color = tint;
		s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint, float tilingFactor, int entityID) {
		NT_PROFILE_FUNCTION();
		DrawQuad({ pos.x, pos.y, 0.0f }, size, subtexture, tint, tilingFactor, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint, float tilingFactor, int entityID) {
		NT_PROFILE_FUNCTION();

		if (s_Data->QuadIndexCount >= s_Data->MaxIndices) {
			EndScene();
			s_Data->TextureSlotIndex = 1;
			s_Data->QuadIndexCount = 0;
			s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
		}

		s_Data->Stats.QuadCount++;

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data->TextureSlotIndex; i++) {
			if (*s_Data->TextureSlots[i] == *subtexture->GetTexture()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data->TextureSlotIndex;
			s_Data->TextureSlots[s_Data->TextureSlotIndex] = subtexture->GetTexture();
			s_Data->TextureSlotIndex++;
		}

		const glm::vec2* texCoords = subtexture->GetTexCoords();

		s_Data->QuadVertexBufferPtr->Position = pos;
		s_Data->QuadVertexBufferPtr->Color = tint;
		s_Data->QuadVertexBufferPtr->TexCoord = texCoords[0];
		s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = { pos.x + size.x, pos.y, 0.0f };
		s_Data->QuadVertexBufferPtr->Color = tint;
		s_Data->QuadVertexBufferPtr->TexCoord = texCoords[1];
		s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = { pos.x + size.x, pos.y + size.y, 0.0f };
		s_Data->QuadVertexBufferPtr->Color = tint;
		s_Data->QuadVertexBufferPtr->TexCoord = texCoords[2];
		s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = { pos.x, pos.y + size.y, 0.0f };
		s_Data->QuadVertexBufferPtr->Color = tint;
		s_Data->QuadVertexBufferPtr->TexCoord = texCoords[3];
		s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		if (s_Data->QuadIndexCount >= s_Data->MaxIndices) {
			EndScene();
			s_Data->TextureSlotIndex = 1;
			s_Data->QuadIndexCount = 0;
			s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
		}

		s_Data->Stats.QuadCount++;

		s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[0];
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = 0;
		s_Data->QuadVertexBufferPtr->TilingFactor = 1;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[1];
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = 0;
		s_Data->QuadVertexBufferPtr->TilingFactor = 1;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[2];
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = 0;
		s_Data->QuadVertexBufferPtr->TilingFactor = 1;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[3];
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = 0;
		s_Data->QuadVertexBufferPtr->TilingFactor = 1;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const glm::vec4& color, int entityID)
	{
		NT_PROFILE_FUNCTION();
		DrawQuad({ pos.x, pos.y, 0.0f }, size, rotation, color, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& color, int entityID)
	{
		NT_PROFILE_FUNCTION();

		if (s_Data->QuadIndexCount >= s_Data->MaxIndices) {
			EndScene();
			s_Data->TextureSlotIndex = 1;
			s_Data->QuadIndexCount = 0;
			s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
		}

		s_Data->Stats.QuadCount++;

		if (rotation == 0) {
			DrawQuad(pos, size, color);
			return;
		}

		glm::mat4 MLM =
			glm::translate(glm::mat4(1.0f), pos) *
			glm::rotate(glm::mat4(1.0f), glm::radians(-rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data->QuadVertexBufferPtr->Position = MLM * s_Data->QuadVertexPositions[0];
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = 0;
		s_Data->QuadVertexBufferPtr->TilingFactor = 1;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = MLM * s_Data->QuadVertexPositions[1];
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = 0;
		s_Data->QuadVertexBufferPtr->TilingFactor = 1;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = MLM * s_Data->QuadVertexPositions[2];
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = 0;
		s_Data->QuadVertexBufferPtr->TilingFactor = 1;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = MLM * s_Data->QuadVertexPositions[3];
		s_Data->QuadVertexBufferPtr->Color = color;
		s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = 0;
		s_Data->QuadVertexBufferPtr->TilingFactor = 1;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor, int entityID)
	{
		NT_PROFILE_FUNCTION();
		DrawQuad({ pos.x, pos.y, 0.0f }, size, rotation, texture, tint, tilingFactor, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor, int entityID)
	{
		NT_PROFILE_FUNCTION();

		if (s_Data->QuadIndexCount >= s_Data->MaxIndices) {
			EndScene();
			s_Data->TextureSlotIndex = 1;
			s_Data->QuadIndexCount = 0;
			s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
		}

		s_Data->Stats.QuadCount++;

		if (rotation == 0) {
			DrawQuad(pos, size, texture, tint, tilingFactor);
			return;
		}

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data->TextureSlotIndex; i++) {
			if (*s_Data->TextureSlots[i] == *texture) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data->TextureSlotIndex;
			s_Data->TextureSlots[s_Data->TextureSlotIndex] = texture;
			s_Data->TextureSlotIndex++;
		}

		glm::mat4 MLM = 
			glm::translate(glm::mat4(1.0f), pos) *
			glm::rotate(glm::mat4(1.0f), glm::radians(-rotation), {0.0f, 0.0f, 1.0f}) *
			glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		s_Data->QuadVertexBufferPtr->Position = MLM * s_Data->QuadVertexPositions[0];
		s_Data->QuadVertexBufferPtr->Color = tint;
		s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = MLM * s_Data->QuadVertexPositions[1];
		s_Data->QuadVertexBufferPtr->Color = tint;
		s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = MLM * s_Data->QuadVertexPositions[2];
		s_Data->QuadVertexBufferPtr->Color = tint;
		s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = MLM * s_Data->QuadVertexPositions[3];
		s_Data->QuadVertexBufferPtr->Color = tint;
		s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint, float tilingFactor, int entityID) {
		NT_PROFILE_FUNCTION();
		DrawQuad({ pos.x, pos.y, 0.0f }, size, rotation, subtexture, tint, tilingFactor, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint, float tilingFactor, int entityID) {
		NT_PROFILE_FUNCTION();

		if (s_Data->QuadIndexCount >= s_Data->MaxIndices) {
			EndScene();
			s_Data->TextureSlotIndex = 1;
			s_Data->QuadIndexCount = 0;
			s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
		}

		s_Data->Stats.QuadCount++;


		if (rotation == 0) {
			DrawQuad(pos, size, subtexture->GetTexture(), tint, tilingFactor);
			return;
		}

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data->TextureSlotIndex; i++) {
			if (*s_Data->TextureSlots[i] == *subtexture->GetTexture()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data->TextureSlotIndex;
			s_Data->TextureSlots[s_Data->TextureSlotIndex] = subtexture->GetTexture();
			s_Data->TextureSlotIndex++;
		}

		const glm::vec2* texCoords = subtexture->GetTexCoords();

		glm::mat4 MLM =
			glm::translate(glm::mat4(1.0f), pos) *
			glm::rotate(glm::mat4(1.0f), glm::radians(-rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data->QuadVertexBufferPtr->Position = MLM * s_Data->QuadVertexPositions[0];
		s_Data->QuadVertexBufferPtr->Color = tint;
		s_Data->QuadVertexBufferPtr->TexCoord = texCoords[0];
		s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = MLM * s_Data->QuadVertexPositions[1];
		s_Data->QuadVertexBufferPtr->Color = tint;
		s_Data->QuadVertexBufferPtr->TexCoord = texCoords[1];
		s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = MLM * s_Data->QuadVertexPositions[2];
		s_Data->QuadVertexBufferPtr->Color = tint;
		s_Data->QuadVertexBufferPtr->TexCoord = texCoords[2];
		s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadVertexBufferPtr->Position = MLM * s_Data->QuadVertexPositions[3];
		s_Data->QuadVertexBufferPtr->Color = tint;
		s_Data->QuadVertexBufferPtr->TexCoord = texCoords[3];
		s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data->QuadVertexBufferPtr->EntityID = entityID;
		s_Data->QuadVertexBufferPtr++;

		s_Data->QuadIndexCount += 6;
	}
	
	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data->Stats;
	}

	void Renderer2D::ResetStats()
	{
		s_Data->Stats.DrawCalls = 0;
		s_Data->Stats.QuadCount = 0;
	}
}
