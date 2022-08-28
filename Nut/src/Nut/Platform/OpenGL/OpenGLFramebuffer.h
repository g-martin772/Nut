#pragma once

#include "Nut/Renderer/Framebuffer.h"

namespace Nut {
	class OpenGLFrameBuffer : public FrameBuffer{
	public:
		OpenGLFrameBuffer(FrameBufferSpecs specs);
		~OpenGLFrameBuffer();

		const FrameBufferSpecs& GetSpecs() const override { return m_Specs; }
		uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const
			{ NT_CORE_ASSERT(index < m_ColorAttachments.size(), "invalid color attachment index"); return m_ColorAttachments[index]; }

		void Invalidate();
		void Resize(uint32_t width, uint32_t height) override;
		int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		void ClearAttachment(uint32_t attachmentIndex, int value) override;

		void Bind() override;
		void UnBind() override;

	private:
		uint32_t m_RendererID = 0;
		FrameBufferSpecs m_Specs;

		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecifications;
		FrameBufferTextureSpecification m_DepthAttachmentSpecification;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}
