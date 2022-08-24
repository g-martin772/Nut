#include "ntpch.h"
#include "OpenGLTexture.h"

#include <../vendor/GLAD/include/glad/glad.h>
#include "stb_image.h"

namespace Nut {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_path(path)
	{
		int width, height, channels = 0;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		NT_CORE_ASSERT(data, "Failed to load image");
		m_Width = width; m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

		if(channels == 3)
			glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);
		else if (channels == 4)
			glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);
		else NT_CORE_ASSERT(false, "Invalid texture channel! Only RGB and RGBA supported!")

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		if (channels == 3)
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (channels == 4)
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else NT_CORE_ASSERT(false, "Invalid texture channel! Only RGB and RGBA supported!")
		

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}
