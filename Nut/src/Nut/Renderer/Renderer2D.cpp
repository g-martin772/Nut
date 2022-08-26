#include "ntpch.h"
#include "Renderer2D.h"
#include <Nut/Renderer/VertexArray.h>
#include <Nut/Renderer/Shader.h>
#include <Nut/Renderer/RenderCommand.h>

#include <../vendor/glm/glm/gtc/matrix_transform.hpp>

namespace Nut {

	struct Renderer2DStorage {
		Ref<VertexArray> VertexArray;
		Ref<Shader> TextureShader2D;
		Ref<Texture2D> WhitePixelx1Texture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		NT_PROFILE_FUNCTION();
		s_Data = new Renderer2DStorage;

		s_Data->TextureShader2D = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->VertexArray = VertexArray::Create();

		float vertices[] = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Pos" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		};
		vertexBuffer->SetLayout(layout);
		s_Data->VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indices[] = { 0, 1, 3, 2, 1, 3 };
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, 6);

		s_Data->WhitePixelx1Texture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhitePixelx1Texture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		s_Data->VertexArray->AddIndexBuffer(indexBuffer);
		s_Data->TextureShader2D->Bind();
		s_Data->TextureShader2D->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->TextureShader2D->Bind();
		s_Data->TextureShader2D->SetMat4("u_VPM", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
	{
		NT_PROFILE_FUNCTION();
		DrawQuad({ pos.x, pos.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color)
	{
		NT_PROFILE_FUNCTION();
		s_Data->TextureShader2D->SetFloat4("u_Color", color);
		s_Data->TextureShader2D->SetFloat("u_TileFactor", 1);
		s_Data->WhitePixelx1Texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)  *  glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		s_Data->TextureShader2D->SetMat4("u_MLM", transform);

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor)
	{
		NT_PROFILE_FUNCTION();
		DrawQuad({ pos.x, pos.y, 0.0f }, size, texture, tint, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor)
	{
		NT_PROFILE_FUNCTION();
		s_Data->TextureShader2D->SetFloat4("u_Color", tint);
		s_Data->TextureShader2D->SetFloat("u_TileFactor", tilingFactor);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)  *  glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader2D->SetMat4("u_MLM", transform);

		texture->Bind();
		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		NT_PROFILE_FUNCTION();
		DrawQuad({ pos.x, pos.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		NT_PROFILE_FUNCTION();
		s_Data->TextureShader2D->SetFloat4("u_Color", color);
		s_Data->TextureShader2D->SetFloat("u_TileFactor", 1);
		s_Data->WhitePixelx1Texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)  *  glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f })  *  glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f });
		s_Data->TextureShader2D->SetMat4("u_MLM", transform);

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor)
	{
		NT_PROFILE_FUNCTION();
		DrawQuad({ pos.x, pos.y, 0.0f }, size, rotation, texture, tint, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor)
	{
		NT_PROFILE_FUNCTION();
		s_Data->TextureShader2D->SetFloat4("u_Color", tint);
		s_Data->TextureShader2D->SetFloat("u_TileFactor", tilingFactor);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)  *  glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f })  *  glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f});
		s_Data->TextureShader2D->SetMat4("u_MLM", transform);

		texture->Bind();
		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}
}
