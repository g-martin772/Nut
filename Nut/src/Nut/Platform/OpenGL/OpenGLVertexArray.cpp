#include "ntpch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

static GLenum ShaderDataTypeToOpenGLBaseType(Nut::ShaderDataType type) {
	switch (type)
	{
	case Nut::ShaderDataType::Float:			return GL_FLOAT;
	case Nut::ShaderDataType::Float2:		return GL_FLOAT;
	case Nut::ShaderDataType::Float3:		return GL_FLOAT;
	case Nut::ShaderDataType::Float4:		return GL_FLOAT;
	case Nut::ShaderDataType::Mat3:			return GL_FLOAT;
	case Nut::ShaderDataType::Mat4:			return GL_FLOAT;
	case Nut::ShaderDataType::Int:			return GL_INT;
	case Nut::ShaderDataType::Int2:			return GL_INT;
	case Nut::ShaderDataType::Int3:			return GL_INT;
	case Nut::ShaderDataType::Int4:			return GL_INT;
	case Nut::ShaderDataType::Bool:			return GL_BOOL;
	default:
		NT_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}
	return 0;
}

Nut::OpenGLVertexArray::OpenGLVertexArray()
{
	NT_PROFILE_FUNCTION();
	glCreateVertexArrays(1, &m_RendererID);
}

Nut::OpenGLVertexArray::~OpenGLVertexArray()
{	
	NT_PROFILE_FUNCTION();
	glDeleteVertexArrays(1, &m_RendererID);
}

void Nut::OpenGLVertexArray::Bind() const
{
	NT_PROFILE_FUNCTION();
	glBindVertexArray(m_RendererID);
}

void Nut::OpenGLVertexArray::UnBind() const
{
	NT_PROFILE_FUNCTION();
	glBindVertexArray(0);

}

void Nut::OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
{
	NT_PROFILE_FUNCTION();
	glBindVertexArray(m_RendererID);
	vertexBuffer->Bind();

	NT_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "VertexBuffer has no layout!");

	uint32_t index = 0;
	for (const auto& element : vertexBuffer->GetLayout()) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, vertexBuffer->GetLayout().GetStride(), (const void*)element.Offset);
		index++;
	}
	m_VertexBuffers.push_back(vertexBuffer);
}

void Nut::OpenGLVertexArray::AddIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
{
	NT_PROFILE_FUNCTION();
	glBindVertexArray(m_RendererID);
	indexBuffer->Bind();

	m_IndexBuffer = indexBuffer;
}
