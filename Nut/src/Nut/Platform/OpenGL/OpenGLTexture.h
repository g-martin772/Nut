#pragma once

#include <Nut/Renderer/Texture.h>

#include <glad/glad.h>

namespace Nut {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }
		std::string GetPath() const override { return m_path; }
		uint32_t GetRendererID() const { return m_RendererID; }

		void SetData(void* data, uint32_t size) override;

		void Bind(uint32_t slot = 0) const override;

		bool operator==(const Texture& other) const override { return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID; }
	private:
		std::string m_path;
		uint32_t m_Width, m_Height = 0;
		uint32_t m_RendererID = 0;
		GLenum m_InternalFormat, m_DataFormat;
	};
}